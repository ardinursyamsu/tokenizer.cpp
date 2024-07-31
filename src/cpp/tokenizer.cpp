#include "org_llama_Tokenizer.h"
#include "common.h"
#include "llama.h"
#include <iostream>

jfieldID f_model_pointer = nullptr;

struct llama_tokenizer
{
    llama_model * model;
    llama_context * ctx;
};

static void llama_log_callback_null(ggml_log_level level, const char * text, void * user_data) {
    (void) level;
    (void) text;
    (void) user_data;
}

/**
 * Convert a Java string to a std::string
 */
std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}


/**
 * Since Java expects utf16 but std::strings are utf8, we can't directly use `env->NewString` or `env-NewString`,
 * but we directly send the bytes and do the conversion in Java. Unfortunately, there isn't a nice/standardized way to
 * do this conversion in C++
 */
jbyteArray parse_jbytes(JNIEnv *env, const std::string &string)
{
    jsize length = string.size(); // NOLINT(*-narrowing-conversions)
    jbyteArray bytes = env->NewByteArray(length);
    env->SetByteArrayRegion(bytes, 0, length, reinterpret_cast<const jbyte *>(string.c_str()));
    return bytes;
}

JNIEXPORT void JNICALL Java_org_llama_Tokenizer_loadModel(JNIEnv *env, jobject obj, jstring modelPath){
    llama_tokenizer *tok = new llama_tokenizer();

    const char *model_path = env->GetStringUTFChars(modelPath, 0);

    bool disable_logging = false;
    if (disable_logging) {
        llama_log_set(llama_log_callback_null, NULL);
    }

    llama_backend_init();

    llama_model_params model_params = llama_model_default_params();

    model_params.vocab_only = true;
    tok->model = llama_load_model_from_file(model_path, model_params);
    if (!tok->model) {
        fprintf(stderr, "Error: could not load model from file '%s'.\n", model_path);
        return;
    }

    llama_context_params ctx_params = llama_context_default_params();
    tok->ctx = llama_new_context_with_model(tok->model, ctx_params);
    if (!tok->ctx) {
        fprintf(stderr, "Error: could not create context.\n");
        return;
    }

    env->ReleaseStringUTFChars(modelPath, model_path);
    env->SetLongField(obj, f_model_pointer, reinterpret_cast<jlong>(tok));
}

JNIEXPORT jbyteArray JNICALL Java_org_llama_Tokenizer_decodeBytes(JNIEnv *env, jobject obj, jintArray jTokens){
    jlong tok_handle = env->GetLongField(obj, f_model_pointer);
    llama_tokenizer *tok = reinterpret_cast<llama_tokenizer *>(tok_handle); // NOLINT(*-no-int-to-ptr)

    jsize length = env->GetArrayLength(jTokens);
    jint *elements = env->GetIntArrayElements(jTokens, nullptr);

    std::vector<llama_token> tokens(elements, elements + length);
    std::string text = ::llama_detokenize(tok->ctx, tokens);

    env->ReleaseIntArrayElements(jTokens, elements, 0);

    return parse_jbytes(env, text);
}

JNIEXPORT jintArray JNICALL Java_org_llama_Tokenizer_tokenize(JNIEnv *env, jobject obj, jstring jprompt){
    jlong tok_handle = env->GetLongField(obj, f_model_pointer);
    llama_tokenizer *tok = reinterpret_cast<llama_tokenizer *>(tok_handle); // NOLINT(*-no-int-to-ptr)

    bool no_bos = false;
    bool no_parse_special = false;
    
    const bool model_wants_add_bos = llama_should_add_bos_token(tok->model);
    const bool add_bos = model_wants_add_bos && !no_bos;
    const bool parse_special = !no_parse_special;

    const std::string prompt = jstring2string(env, jprompt);
    std::vector<llama_token> tokens = ::llama_tokenize(tok->model, prompt, add_bos, parse_special);

    jsize token_size = tokens.size(); // NOLINT(*-narrowing-conversions)

    jintArray java_tokens = env->NewIntArray(token_size);
    if (java_tokens == nullptr)
    {
        // env->ThrowNew(c_error_oom, "could not allocate token memory");
        return nullptr;
    }

    env->SetIntArrayRegion(java_tokens, 0, token_size, reinterpret_cast<const jint *>(tokens.data()));

    return java_tokens;
}

JNIEXPORT void JNICALL Java_org_llama_Tokenizer_closeModel(JNIEnv *env, jobject obj){
    jlong tok_handle = env->GetLongField(obj, f_model_pointer);
    llama_tokenizer *tok = reinterpret_cast<llama_tokenizer *>(tok_handle); // NOLINT(*-no-int-to-ptr)

    // silence valgrind
    llama_free(tok->ctx);
    llama_free_model(tok->model);
}
  
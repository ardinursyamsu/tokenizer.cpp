/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_llama_Tokenizer */

#ifndef _Included_org_llama_Tokenizer
#define _Included_org_llama_Tokenizer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_llama_Tokenizer
 * Method:    loadModel
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_llama_Tokenizer_loadModel
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_llama_Tokenizer
 * Method:    decodeBytes
 * Signature: ([I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_llama_Tokenizer_decodeBytes
  (JNIEnv *, jobject, jintArray);

/*
 * Class:     org_llama_Tokenizer
 * Method:    tokenize
 * Signature: (Ljava/lang/String;)[I
 */
JNIEXPORT jintArray JNICALL Java_org_llama_Tokenizer_tokenize
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_llama_Tokenizer
 * Method:    closeModel
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_llama_Tokenizer_closeModel
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

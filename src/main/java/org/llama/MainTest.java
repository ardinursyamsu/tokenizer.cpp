package org.llama;

public class MainTest {
    public static void main(String[] args) {
        Tokenizer tok = new Tokenizer("D:\\PROJECTAI\\MODELS\\stability\\stable-code\\stable-code-instruct-3b-q8_0.gguf");
        String prompt = "For the simplicity, I like to use llama.cpp";
        int[] tokens = tok.tokenize(prompt);
        String text = tok.decode(tokens);
        System.out.println("prompt: " + prompt);
        System.out.print("tokens: ");
        printIntArray(tokens);
        System.out.println("decode: " + text);
        tok.close();
    }

    private static void printIntArray(int[] intArray){
        String text = "[";
        for (int i : intArray){
            text += String.valueOf(i);
            text += ", ";
        }
        text = text.substring(0, text.length() - 2);
        text += "]";
        System.out.println(text);
    }
}
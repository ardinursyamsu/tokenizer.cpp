Wrapper for llama.cpp but tokenizer only. I used this as Text splitter in tokenizer level. Most code is from tokenize.cpp and java-llama.cpp

### To install use this command
```
cmake -B build
cmake --build build --config Release -j
```

### compile java via maven
```
mvn clean install package
```

### add package to pom.xml file in your maven project
```
<dependency>
    <groupId>org.llama</groupId>
    <artifactId>tokenizer</artifactId>
    <version>1.0.0</version>
</dependency>
```

### Example
```
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
```
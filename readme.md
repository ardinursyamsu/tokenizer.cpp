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

### add package to pom.xml file in you maven project
```
<dependency>
    <groupId>org.llama</groupId>
    <artifactId>tokenizer</artifactId>
    <version>1.0.0</version>
</dependency>
```
Wrapper for llama.cpp but tokenizer only. I used this as Text splitter in tokenizer level. Most code is from tokenize.cpp and java-llama.cpp

### To install use this command
```
cmake -B build
cmake --build build --config Release -j
```

### add dll searchpath
```
$env:PATH+='D:\useful_ai_project\java\tokenizer_llama_cpp\build\bin\Release\'
```
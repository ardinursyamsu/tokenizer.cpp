package org.llama;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class Tokenizer implements AutoCloseable {
    private static boolean loadNativeLibrary(Path path) {
		if (!Files.exists(path)) {
			return false;
		}
		String absolutePath = path.toAbsolutePath().toString();
		try {
			System.load(absolutePath);
			return true;
		}
		catch (UnsatisfiedLinkError e) {
			e.printStackTrace();
			return false;
		}
	}

    private static void loadLibrary(){
        Path path = Path.of("./src/main/resources/lib/tokenizerllama.dll");
        loadNativeLibrary(path);
    }

    static {
        loadLibrary();
    }

    public Tokenizer(String modelPath){
        loadModel(modelPath);
    }

    public String decode(int[] tokens){
        byte[] bytes = decodeBytes(tokens);
		return new String(bytes, StandardCharsets.UTF_8);
    }

    @Override
    public void close(){
        closeModel();
    }

    private native void loadModel(String modelPath);

    private native byte[] decodeBytes(int[] tokens);

    public native int[] tokenize(String prompt);

    public native void closeModel();
}

package org.llama;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class Tokenizer implements AutoCloseable {
    private static boolean loadNativeLibrary(Path path){
		if (!Files.exists(path)) {
            try {
                String nativeLibraryFilePath = "/lib/" + path.toFile().getName(); // this is library file located if run from .jar

                // we now create temp directory to put the libs
                String tmpDir = System.getProperty("java.io.tmpdir") + "/lib/"; // don't collide with java-llama.cpp
                File tmpFolder = new File(tmpDir);
                if (!tmpFolder.exists()){
                    tmpFolder.mkdirs();
                }
                Path extractedFilePath = Path.of(tmpDir, path.toFile().getName());

                // Extract a native library file into the target directory
                try (InputStream reader = Tokenizer.class.getResourceAsStream(nativeLibraryFilePath)) {
                    if (reader == null) {
                        return false;
                    }
                    Files.copy(reader, extractedFilePath, StandardCopyOption.REPLACE_EXISTING);
                } catch (IOException e){
                    e.printStackTrace();

                }
                finally {
                    // Delete the extracted lib file on JVM exit.
                    extractedFilePath.toFile().deleteOnExit();
                }

			    System.load(extractedFilePath.toString());
                return true;
            } catch (UnsatisfiedLinkError e){
                e.printStackTrace();

                return false;
            }
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

        Path path = Path.of("./src/main/resources/lib/ggml.dll");
        loadNativeLibrary(path);
        path = Path.of("./src/main/resources/lib/llama.dll");
        loadNativeLibrary(path);
        path = Path.of("./src/main/resources/lib/tokenizerllama.dll");
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

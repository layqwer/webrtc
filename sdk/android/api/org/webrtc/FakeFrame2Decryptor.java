package org.webrtc;

public class FakeFrame2Decryptor implements FrameDecryptor {
    private final long nativeDecryptor;

    public FakeFrame2Decryptor(String key) {
        nativeDecryptor = nativeGetFakeFrame2Decryptor(key);
    }

    @Override
    public long getNativeFrameDecryptor() {
        return nativeDecryptor;
    }

    private static native long nativeGetFakeFrame2Decryptor(String key);
}
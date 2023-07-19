package org.webrtc;

public class FakeFrame2Encryptor implements FrameEncryptor {
    private final long nativeEncryptor;

    public FakeFrame2Encryptor(String key) {
        nativeEncryptor = nativeGetFakeFrame2Encryptor(key);
    }

    @Override
    public long getNativeFrameEncryptor() {
        return nativeEncryptor;
    }

    private static native long nativeGetFakeFrame2Encryptor(String key);
}
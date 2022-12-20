package org.webrtc;
public class FakeFrame2Decryptor implements FrameDecryptor {
    private final long nativeDecryptor;
    public FakeFrame2Decryptor() {
        nativeDecryptor = nativeGetFakeFrame2Decryptor();
    }
    @Override
    public long getNativeFrameDecryptor() {
        return nativeDecryptor;
    }
    // public void setKey(int[] key) {
    // }
    private static native long nativeGetFakeFrame2Decryptor();
}
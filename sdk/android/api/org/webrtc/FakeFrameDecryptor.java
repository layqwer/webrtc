package org.webrtc;
public class FakeFrameDecryptor implements FrameDecryptor {
    private final long nativeDecryptor;
    public FakeFrameDecryptor() {
        nativeDecryptor = nativeGetFakeFrameDecryptor();
    }
    @Override
    public long getNativeFrameDecryptor() {
        return nativeDecryptor;
    }
    public void setKey(int[] key) {
    }
    private static native long nativeGetFakeFrameDecryptor();
}
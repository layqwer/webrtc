package org.webrtc;
public class FakeFrameEncryptor implements FrameEncryptor {
    private final long nativeEncryptor;
    public FakeFrameEncryptor() {
        nativeEncryptor = nativeGetFakeFrameEncryptor();
    }
    @Override
    public long getNativeFrameEncryptor() {
        return nativeGetFakeFrameEncryptor();
    }
    public void setKey(int[] key) {
        nativeSetKey(key);
    }
    private static native long nativeGetFakeFrameEncryptor();
    private native void nativeSetKey(int[] key);
}
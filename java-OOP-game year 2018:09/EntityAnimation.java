import processing.core.PImage;

public interface EntityAnimation {
    void nextImage();
    int getAnimationPeriod();
    PImage getCurrentImage();
}

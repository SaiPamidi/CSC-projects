import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

public interface Entity{
    void scheduleActions(EventScheduler scheduler,WorldModel world, ImageStore imageStore);
    Point getPosition();
    void setPosition(Point p);
    void nextImage();
    int getAnimationPeriod();
    PImage getCurrentImage();
    String getStringID();

}
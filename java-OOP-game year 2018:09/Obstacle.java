import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

final class Obsatcle implements Entity{

    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;
    //new


    public Obsatcle(String id, Point position,
                    List<PImage> images) {
        this.id = id;
        this.position = position;
        this.images = images;
        this.imageIndex = 0;

    }
    public int getAnimationPeriod() {
        return 0;
    }
    public String getStringID(){
        return id;
    }
    public PImage getCurrentImage() {


        return this.images.get((this).imageIndex);

    }
    public Point getPosition(){
        return position;
    }

    public void setPosition(Point p ){
        this.position = p;

    }
    public void scheduleActions(EventScheduler scheduler,
                                WorldModel world, ImageStore imageStore) {
        return;

    }
    public void nextImage() {
        this.imageIndex = (this.imageIndex + 1) % this.images.size();
    }

    public static Obsatcle createObstacle(String id, Point position,
                                          List<PImage> images) {
        return new Obsatcle(id, position, images);
    }
}


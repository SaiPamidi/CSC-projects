import java.util.List;
import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

final class Quake implements Entity,EntityAnimation{
    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;
    //new

    private static final String QUAKE_ID = "quake";
    private static final int QUAKE_ACTION_PERIOD = 1100;
    private static final int QUAKE_ANIMATION_PERIOD = 100;
    private static final int QUAKE_ANIMATION_REPEAT_COUNT = 10;


    public Quake(String id, Point position,
                 List<PImage> images, int resourceLimit, int resourceCount,
                 int actionPeriod, int animationPeriod) {

        this.id = id;
        this.position = position;
        this.images = images;
        this.imageIndex = 0;
        this.resourceLimit = resourceLimit;
        this.resourceCount = resourceCount;
        this.actionPeriod = actionPeriod;
        this.animationPeriod = animationPeriod;
    }
    public Point getPosition(){
        return position;
    }

    public void setPosition(Point p ){
        this.position = p;

    }
    public String getStringID(){
        return id;
    }

    public PImage getCurrentImage() {


        return this.images.get((this).imageIndex);

    }

    public int getAnimationPeriod() {

        return this.animationPeriod;

    }

    public void nextImage() {
        this.imageIndex = (this.imageIndex + 1) % this.images.size();
    }


    public void executeQuakeActivity(WorldModel world, ImageStore imageStore, EventScheduler scheduler) {
        scheduler.unscheduleAllEvents(this);
        world.removeEntity(this);
    }

    public static Quake createQuake(Point position, List<PImage> images)
    {
        return new Quake(QUAKE_ID, position, images,
                0, 0, QUAKE_ACTION_PERIOD, QUAKE_ANIMATION_PERIOD);
    }

    public void scheduleActions(EventScheduler scheduler,
                                WorldModel world, ImageStore imageStore)
    {


                scheduler.scheduleEvent(this,
                        Activity.createActivityAction(this,world, imageStore),
                        this.actionPeriod);
                scheduler.scheduleEvent(this,
                        Animation.createAnimationAction(this,QUAKE_ANIMATION_REPEAT_COUNT),
                        this.getAnimationPeriod());

    }


}

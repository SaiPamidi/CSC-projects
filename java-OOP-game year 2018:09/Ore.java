import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

final class Ore implements Entity,EntityAnimation {
    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;
    //new
    private static final String BLOB_KEY = "blob";
    private static final String BLOB_ID_SUFFIX = " -- blob";
    private static final int BLOB_PERIOD_SCALE = 4;
    private static final int BLOB_ANIMATION_MIN = 50;
    private static final int BLOB_ANIMATION_MAX = 150;
    private static final Random rand = new Random();




    public Ore(String id, Point position,
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


    public void executeOreActivity(WorldModel world, ImageStore imageStore, EventScheduler scheduler) {
        Point pos = this.position;  // store current position before removing

        world.removeEntity(this);
        scheduler.unscheduleAllEvents(this);

        Entity blob = OreBlob.createOreBlob(this.id + BLOB_ID_SUFFIX,
                pos, this.actionPeriod / BLOB_PERIOD_SCALE,
                BLOB_ANIMATION_MIN +
                        rand.nextInt(BLOB_ANIMATION_MAX - BLOB_ANIMATION_MIN),
                imageStore.getImageList(BLOB_KEY));

        world.addEntity(blob);
        blob.scheduleActions(scheduler, world, imageStore);
    }




    public static Ore createOre(String id, Point position, int actionPeriod,
                                   List<PImage> images)
    {
        return new Ore(id, position, images, 0, 0,
                actionPeriod, 0);
    }


    public void scheduleActions(EventScheduler scheduler,
                                WorldModel world, ImageStore imageStore) {

        scheduler.scheduleEvent(this,
                Activity.createActivityAction(this, world, imageStore),
                this.actionPeriod);
    }





}

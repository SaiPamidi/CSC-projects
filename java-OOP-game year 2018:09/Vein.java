import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

final class Vein implements Entity,EntityAnimation{

    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;
    //new

    private static final Random rand = new Random();



    private static final String ORE_ID_PREFIX = "ore -- ";
    private static final int ORE_CORRUPT_MIN = 20000;
    private static final int ORE_CORRUPT_MAX = 30000;
    private static final String ORE_KEY = "ore";
    private static final int ORE_REACH = 1;




    public Vein(String id, Point position,
                  List<PImage> images, int resourceLimit, int resourceCount,
                  int actionPeriod) {

        this.id = id;
        this.position = position;
        this.images = images;
        this.imageIndex = 0;
        this.resourceLimit = resourceLimit;
        this.resourceCount = resourceCount;
        this.actionPeriod = actionPeriod;
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
        return animationPeriod;
    }

    public void nextImage() {
        this.imageIndex = (this.imageIndex + 1) % this.images.size();
    }



    public void executeVeinActivity(WorldModel world, ImageStore imageStore, EventScheduler scheduler) {
        Optional<Point> openPt =this.findOpenAround(world,this.position);

        if (openPt.isPresent()) {
            Entity ore = Ore.createOre(ORE_ID_PREFIX + this.id,
                    openPt.get(), ORE_CORRUPT_MIN +
                            rand.nextInt(ORE_CORRUPT_MAX - ORE_CORRUPT_MIN),
                    imageStore.getImageList(ORE_KEY));
            world.addEntity(ore);
            ore.scheduleActions(scheduler,world, imageStore);
        }

        scheduler.scheduleEvent(this,
                Activity.createActivityAction(this,world, imageStore),
                this.actionPeriod);
    }
    private static Optional<Point> findOpenAround(WorldModel world, Point pos)
    {
        for (int dy = -ORE_REACH; dy <= ORE_REACH; dy++)
        {
            for (int dx = -ORE_REACH; dx <= ORE_REACH; dx++)
            {
                Point newPt = new Point(pos.x + dx, pos.y + dy);
                if (world.withinBounds(newPt) &&
                        !world.isOccupied(newPt))
                {
                    return Optional.of(newPt);
                }
            }
        }

        return Optional.empty();
    }

    public static Vein createVein(String id, Point position, int actionPeriod,
                                    List<PImage> images)
    {
        return new Vein( id, position, images, 0, 0,
                actionPeriod);
    }


    public void scheduleActions(EventScheduler scheduler,
                                       WorldModel world, ImageStore imageStore)
    {
                scheduler.scheduleEvent(this,
                        Activity.createActivityAction(this, world, imageStore),
                        this.actionPeriod);

    }

}





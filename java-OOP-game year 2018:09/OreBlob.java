import java.util.List;
import java.util.Optional;
import java.util.Random;

import processing.core.PImage;

final class OreBlob implements Entity,EntityAnimation {
    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;
    private static final int ORE_REACH = 1;
    private static final String QUAKE_KEY = "quake";


    public OreBlob(String id, Point position,
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

    public PImage getCurrentImage() {


        return this.images.get((this).imageIndex);

    }

    public int getAnimationPeriod() {

        return this.animationPeriod;
    }

    public void nextImage() {
        this.imageIndex = (this.imageIndex + 1) % this.images.size();
    }


    public void executeOreBlobActivity(WorldModel world, ImageStore imageStore, EventScheduler scheduler) {
        Optional<Entity> blobTarget = world.findNearest(
                this.position, "Vein");
        long nextPeriod = this.actionPeriod;

        if (blobTarget.isPresent()) {
            Point tgtPos = blobTarget.get().getPosition();

            if (moveToOreBlob(this, world, blobTarget.get(), scheduler)) {
                Entity quake = Quake.createQuake(tgtPos,
                        imageStore.getImageList(QUAKE_KEY));

                world.addEntity(quake);
                nextPeriod += this.actionPeriod;
                quake.scheduleActions(scheduler, world, imageStore);
            }
        }

        scheduler.scheduleEvent(this,
                Activity.createActivityAction(this, world, imageStore),
                nextPeriod);
    }

    public String getStringID(){
        return id;
    }


    private static boolean moveToOreBlob(Entity blob, WorldModel world,
                                         Entity target, EventScheduler scheduler)           //not sure
    {
        if (adjacent(blob.getPosition(), target.getPosition()))
        {
            world.removeEntity(target);
            scheduler.unscheduleAllEvents(target);
            return true;
        }
        else
        {
            Point nextPos = ((OreBlob)blob).nextPositionOreBlob(world, target.getPosition());

            if (!blob.getPosition().equals(nextPos))
            {
                Optional<Entity> occupant = world.getOccupant(nextPos);
                if (occupant.isPresent())
                {
                    scheduler.unscheduleAllEvents(occupant.get());
                }

                world.moveEntity(blob, nextPos);
            }
            return false;
        }
    }
    private static boolean adjacent(Point p1, Point p2)
    {
        return (p1.x == p2.x && Math.abs(p1.y - p2.y) == 1) ||
                (p1.y == p2.y && Math.abs(p1.x - p2.x) == 1);
    }


    public static OreBlob createOreBlob(String id, Point position,
                                       int actionPeriod, int animationPeriod, List<PImage> images)
    {
        return new OreBlob(id, position, images,
                0, 0, actionPeriod, animationPeriod);
    }


    public void scheduleActions(EventScheduler scheduler,
                                WorldModel world, ImageStore imageStore) {


        scheduler.scheduleEvent(this,
                Activity.createActivityAction(this, world, imageStore),
                this.actionPeriod);
        scheduler.scheduleEvent(this, Animation.createAnimationAction(this, 0),
                this.getAnimationPeriod());
    }

    private Point nextPositionOreBlob( WorldModel world,
                                       Point destPos)
    {
        int horiz = Integer.signum(destPos.x - this.position.x);
        Point newPos = new Point(this.position.x + horiz,
                this.position.y);

        Optional<Entity> occupant = world.getOccupant(newPos);

        if (horiz == 0 ||
                (occupant.isPresent() && !(occupant.get() instanceof Ore)))
        {
            int vert = Integer.signum(destPos.y - this.position.y);
            newPos = new Point(this.position.x, this.position.y + vert);
            occupant = world.getOccupant(newPos);

            if (vert == 0 ||
                    (occupant.isPresent() && !(occupant.get() instanceof Ore)))
            {
                newPos = this.position;
            }
        }

        return newPos;
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






}

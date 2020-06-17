import java.util.List;
import java.util.Optional;


import processing.core.PImage;

final class MinerFull implements Entity,Miner,EntityAnimation{
    public String id;
    public Point position;
    public List<PImage> images;
    public int imageIndex;
    public int resourceLimit;
    public int resourceCount;
    public int actionPeriod;
    public int animationPeriod;




    public MinerFull(String id, Point position,
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
    public int getresourceCount(){
        return resourceCount;

    }

    public void setresourceCount(int a){
        this.resourceCount = a;

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

    public void executeMinerFullActivity(WorldModel world, ImageStore imageStore, EventScheduler scheduler) {
        Optional<Entity> fullTarget = world.findNearest(this.position,
                "Blacksmith");

        if (fullTarget.isPresent() &&
                moveToFull(this, world, fullTarget.get(), scheduler)) {
            transformFull(world, scheduler, imageStore);
        } else {
            scheduler.scheduleEvent(this,
                    Activity.createActivityAction(this,world, imageStore),
                    this.actionPeriod);
        }
    }
    public void scheduleActions(EventScheduler scheduler,
                                WorldModel world, ImageStore imageStore)
    {

                scheduler.scheduleEvent(this,
                        Activity.createActivityAction(this,world, imageStore),
                        this.actionPeriod);
                scheduler.scheduleEvent(this, Animation.createAnimationAction(this, 0),
                        this.getAnimationPeriod());



    }



    private  boolean moveToFull(Miner miner, WorldModel world,
                                      Entity target, EventScheduler scheduler)
    {
        if (adjacent(miner.getPosition(), target.getPosition()))
        {
            return true;
        }
        else
        {
            Point nextPos = miner.nextPositionMiner(world, target.getPosition());

            if (!miner.getPosition().equals(nextPos))
            {
                Optional<Entity> occupant = world.getOccupant(nextPos);
                if (occupant.isPresent())
                {
                    scheduler.unscheduleAllEvents(occupant.get());
                }

                world.moveEntity((Entity)miner,nextPos);
            }
            return false;
        }
    }
    public Point nextPositionMiner(WorldModel world,
                                    Point destPos)
    {
        int horiz = Integer.signum(destPos.x - this.position.x);
        Point newPos = new Point(this.position.x + horiz,
                this.position.y);

        if (horiz == 0 || world.isOccupied(newPos))
        {
            int vert = Integer.signum(destPos.y - this.position.y);
            newPos = new Point(this.position.x,
                    this.position.y + vert);

            if (vert == 0 || world.isOccupied(newPos))
            {
                newPos = this.position;
            }
        }

        return newPos;
    }

    private static boolean adjacent(Point p1, Point p2)
    {
        return (p1.x == p2.x && Math.abs(p1.y - p2.y) == 1) ||
                (p1.y == p2.y && Math.abs(p1.x - p2.x) == 1);
    }

    private void transformFull(WorldModel world,
                               EventScheduler scheduler, ImageStore imageStore)
    {
        Entity miner = MinerNotFull.createMinerNotFull(this.id, this.resourceLimit,
                this.position, this.actionPeriod, this.animationPeriod,
                this.images);

        world.removeEntity(this);
        scheduler.unscheduleAllEvents(this);

        world.addEntity(miner);
        miner.scheduleActions(scheduler,world, imageStore);
    }

    public static MinerFull createMinerFull(String id, int resourceLimit,
                                         Point position, int actionPeriod, int animationPeriod,
                                         List<PImage> images)
    {
        return new MinerFull(id, position, images,
                resourceLimit, resourceLimit, actionPeriod, animationPeriod);
    }






}




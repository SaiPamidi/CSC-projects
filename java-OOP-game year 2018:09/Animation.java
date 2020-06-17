import java.util.Optional;

final class Animation implements Action  {
    public Entity entity;
    public WorldModel world;
    private int repeatCount;

    public Animation(Entity entity, WorldModel world,int repeatCount) {
        this.entity = entity;
        this.world = world;
        this.repeatCount = repeatCount;
    }

    public void executeAction(EventScheduler scheduler){
        executeAnimationAction(scheduler);
        }


    private void executeAnimationAction(EventScheduler scheduler) {
        this.entity.nextImage();

        if (this.repeatCount != 1) {
            scheduler.scheduleEvent(this.entity,
                    this.createAnimationAction(entity,
                            Math.max(this.repeatCount - 1, 0)),
                    this.entity.getAnimationPeriod());
        }
    }



    public static Animation createAnimationAction(Entity entity, int repeatCount)
    {
        return new Animation(entity, null, repeatCount);
    }

}



import java.util.Optional;

final class Activity implements Action {
    public Entity entity;
    public WorldModel world;
    private ImageStore imageStore;

    public Activity(Entity entity, WorldModel world,
                  ImageStore imageStore) {
        this.entity = entity;
        this.world = world;
        this.imageStore = imageStore;
    }

    public void executeAction(EventScheduler scheduler) {
                executeActivityAction(scheduler);
    }


    private void executeActivityAction(EventScheduler scheduler) {
        if(this.entity instanceof MinerFull){
            ((MinerFull)this.entity).executeMinerFullActivity(this.world,
                    this.imageStore, scheduler);

        }

        else if(this.entity instanceof MinerNotFull) {
            ((MinerNotFull) this.entity).executeMinerNotFullActivity(this.world,
                    this.imageStore, scheduler);
        }

        else if(this.entity instanceof Ore) {
            ((Ore) this.entity).executeOreActivity(this.world,
                    this.imageStore, scheduler);
        }

        else if(this.entity instanceof OreBlob) {
            ((OreBlob) this.entity).executeOreBlobActivity(this.world,
                    this.imageStore, scheduler);
        }

        else if(this.entity instanceof Quake) {
            ((Quake) this.entity).executeQuakeActivity(this.world,
                    this.imageStore, scheduler);
        }

        else if(this.entity instanceof Vein) {
            ((Vein) this.entity).executeVeinActivity(this.world,
                    this.imageStore, scheduler);
        }

        else {
            throw new UnsupportedOperationException(
                    String.format("executeActivityAction not supported for %s",
                            this.entity.getClass()));
        }








        }

    public static Activity createActivityAction(Entity entity, WorldModel world,
                                              ImageStore imageStore)
    {
        return new Activity(entity, world, imageStore);
    }
}



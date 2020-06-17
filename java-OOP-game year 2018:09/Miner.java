public interface Miner {
    Point nextPositionMiner(WorldModel world,Point destPos);
    Point getPosition();
    void setPosition(Point p);
    int getresourceCount();
    void setresourceCount(int a);


}

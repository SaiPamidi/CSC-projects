import processing.core.PImage;

import java.util.*;

final class WorldModel
{
   public int numRows;
   public int numCols;
   public Background background[][];
   public Entity occupancy[][];
   public Set<Entity> entities;

   //new


   public WorldModel(int numRows, int numCols, Background defaultBackground)
   {
      this.numRows = numRows;
      this.numCols = numCols;
      this.background = new Background[numRows][numCols];
      this.occupancy = new Entity[numRows][numCols];
      this.entities = new HashSet<>();

      for (int row = 0; row < numRows; row++)
      {
         Arrays.fill(this.background[row], defaultBackground);
      }
   }



   public   boolean withinBounds(Point pos)
   {
      return pos.y >= 0 && pos.y < this.numRows &&
              pos.x >= 0 && pos.x < this.numCols;
   }

   public boolean isOccupied(Point pos)
   {
      return this.withinBounds(pos) &&
              this.getOccupancyCell(pos) != null;
   }



   private Optional<Entity> nearestEntity(List<Entity> entities,
                                                Point pos)
   {
      if (entities.isEmpty())
      {
         return Optional.empty();
      }
      else
      {
         Entity nearest = entities.get(0);
         int nearestDistance = distanceSquared(nearest.getPosition(), pos);

         for (Entity other : entities)
         {
            int otherDistance = distanceSquared(other.getPosition(), pos);

            if (otherDistance < nearestDistance)
            {
               nearest = other;
               nearestDistance = otherDistance;
            }
         }

         return Optional.of(nearest);
      }
   }

   private   int distanceSquared(Point p1, Point p2)
   {
      int deltaX = p1.x - p2.x;
      int deltaY = p1.y - p2.y;

      return deltaX * deltaX + deltaY * deltaY;
   }

   public   Optional<Entity> findNearest(Point pos,
                                              String kind)
   {
      List<Entity> ofType = new LinkedList<>();
      for (Entity entity : this.entities)
      {
         //System.out.println(((entity.getClass()).toString()).substring(5));
         //System.out.println(kind);
         if (((entity.getClass()).toString()).substring(6).equals(kind))
         {

            ofType.add(entity);
         }
      }

      return nearestEntity(ofType, pos);
   }

   /*
      Assumes that there is no entity currently occupying the
      intended destination cell.
   */






   public Optional<PImage> getBackgroundImage(Point pos)
   {
      if (this.withinBounds(pos))
      {
         return Optional.of(this.getBackgroundCell(pos).getCurrentImage());
      }
      else
      {
         return Optional.empty();
      }
   }

   public void setBackground(Point pos,
                                    Background background)
   {
      if (this.withinBounds(pos))
      {
         this.setBackgroundCell(pos, background);
      }
   }

   public Optional<Entity> getOccupant(Point pos)
   {
      if (this.isOccupied(pos))
      {
         return Optional.of(this.getOccupancyCell(pos));
      }
      else
      {
         return Optional.empty();
      }
   }

   public Entity getOccupancyCell(Point pos)
   {
      return this.occupancy[pos.y][pos.x];
   }

   public void setOccupancyCell(Point pos,
                                       Entity entity)
   {
      this.occupancy[pos.y][pos.x] = entity;
   }

   private   Background getBackgroundCell(Point pos)
   {
      return this.background[pos.y][pos.x];
   }

   private void setBackgroundCell( Point pos,
                                        Background background)
   {
      this.background[pos.y][pos.x] = background;
   }

   public void tryAddEntity(Entity entity)
   {
      if (this.isOccupied(entity.getPosition()))
      {
         // arguably the wrong type of exception, but we are not
         // defining our own exceptions yet
         throw new IllegalArgumentException("position occupied");
      }

      this.addEntity(entity);
   }
   public void addEntity(Entity entity)
   {
      if (this.withinBounds(entity.getPosition()))
      {
         this.setOccupancyCell(entity.getPosition(), entity);
         this.entities.add(entity);
      }
   }

   public void removeEntity(Entity entity)
   {
      this.removeEntityAt(entity.getPosition());
   }
   private void removeEntityAt(Point pos)
   {
      if (this.withinBounds(pos)
              && this.getOccupancyCell(pos) != null)
      {
         Entity entity = this.getOccupancyCell(pos);

         /* this moves the entity just outside of the grid for
            debugging purposes */
         entity.setPosition (new Point(-1, -1));
         this.entities.remove(entity);
         this.setOccupancyCell(pos, null);
      }
   }

   public void moveEntity(Entity entity, Point pos)
   {
      Point oldPos = entity.getPosition();
      if (this.withinBounds(pos) && !pos.equals(oldPos))
      {
         this.setOccupancyCell(oldPos, null);
         this.removeEntityAt(pos);
         this.setOccupancyCell(pos, entity);
         entity.setPosition(pos);
      }
   }
}




import processing.core.PApplet;
import processing.core.PImage;

import java.util.Optional;

final class WorldView
{
   public PApplet screen;
   public WorldModel world;
   public int tileWidth;
   public int tileHeight;
   public Viewport viewport;

   public WorldView(int numRows, int numCols, PApplet screen, WorldModel world,
      int tileWidth, int tileHeight)
   {
      this.screen = screen;
      this.world = world;
      this.tileWidth = tileWidth;
      this.tileHeight = tileHeight;
      this.viewport = new Viewport(numRows, numCols);
   }
   /*private   Point viewportToWorld(Viewport viewport, int col, int row)
   {
      return new Point(col + viewport.col, row + viewport.row);
   }

   private  Point worldToViewport(Viewport viewport, int col, int row)
   {
      return new Point(col - viewport.col, row - viewport.row);
   }*/
   private  void drawBackground(WorldView view)
   {
      for (int row = 0; row < view.viewport.numRows; row++)
      {
         for (int col = 0; col < view.viewport.numCols; col++)
         {
            Point worldPoint = view.viewport.viewportToWorld(col, row);
            Optional<PImage> image = view.world.getBackgroundImage(
                    worldPoint);
            if (image.isPresent())
            {
               view.screen.image(image.get(), col * view.tileWidth,
                       row * view.tileHeight);
            }
         }
      }
   }

   private  void drawEntities(WorldView worldView)
   {
      for (Entity entity : this.world.entities)
      {
         Point pos = entity.getPosition();

         if (this.viewport.contains(pos))
         {
            Point viewPoint = this.viewport.worldToViewport(pos.x, pos.y);
            this.screen.image(entity.getCurrentImage(),
                    viewPoint.x * this.tileWidth, viewPoint.y * this.tileHeight);
         }
      }
   }

   public void drawViewport()
   {
      drawBackground(this);
      drawEntities(this);
   }
   private int clamp(int value, int low, int high)
   {
      return Math.min(high, Math.max(value, low));
   }

   public void shiftView( int colDelta, int rowDelta)
   {
      int newCol = clamp(this.viewport.col + colDelta, 0,
              this.world.numCols - this.viewport.numCols);
      int newRow = clamp(this.viewport.row + rowDelta, 0,
              this.world.numRows - this.viewport.numRows);

      this.viewport.shift(newCol, newRow);
   }

}

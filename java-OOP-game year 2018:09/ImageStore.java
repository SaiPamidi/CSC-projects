import java.util.*;

import processing.core.PApplet;
import processing.core.PImage;

final class ImageStore
{
   public Map<String, List<PImage>> images;
   private List<PImage> defaultImages;
   //new




   public ImageStore(PImage defaultImage)
   {
      this.images = new HashMap<>();
      defaultImages = new LinkedList<>();
      defaultImages.add(defaultImage);
   }
   public   List<PImage> getImageList(String key)
   {
      return this.images.getOrDefault(key, this.defaultImages);
   }


}

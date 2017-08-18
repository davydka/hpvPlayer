 
import java.awt.Robot;
import java.awt.image.BufferedImage;
import java.awt.Rectangle;

import com.heroicrobot.dropbit.registry.*;
import com.heroicrobot.dropbit.devices.pixelpusher.Pixel;
import com.heroicrobot.dropbit.devices.pixelpusher.Strip;

import processing.core.*;
import java.util.*;

DeviceRegistry registry;
TestObserver testObserver;
 
Robot robot;
 
void setup() {
  size(320, 320, P2D);
  registry = new DeviceRegistry();
  testObserver = new TestObserver();
  registry.addObserver(testObserver);
  
  try {
    robot = new Robot();
  } 
  catch (Exception e) {
    println(e.getMessage());
  }
}
 
void draw() {
  background(0);
  //Rectangle r = new Rectangle(mouseX, mouseY, width, height);
  Rectangle r = new Rectangle(0, 30, 320, 320);
  BufferedImage img1 = robot.createScreenCapture(r);
  PImage img2 = new PImage(img1);
  translate(0, 320);
  rotate(radians(-90));
  image(img2, 0, 0);
  
  scrape();
}

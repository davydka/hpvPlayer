 
import java.awt.Robot;
import java.awt.image.BufferedImage;
import java.awt.Rectangle;
 
Robot robot;
 
void setup() {
  size(320, 320);
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
  Rectangle r = new Rectangle(0, 22, 320, 320);
  BufferedImage img1 = robot.createScreenCapture(r);
  PImage img2 = new PImage(img1);
  image(img2, 0, 0);
}

// pixelpusher syphon sketch.
// takes pixels from syphon, puts 'em on the pixelpusher array.
// jas strong, 6th Dec 2013.

import codeanticode.syphon.*;

import com.heroicrobot.dropbit.registry.*;
import com.heroicrobot.dropbit.devices.pixelpusher.Pixel;
import com.heroicrobot.dropbit.devices.pixelpusher.Strip;

import processing.core.*;
import java.util.*;

DeviceRegistry registry;

SyphonClient client;
PGraphics canvas;

boolean ready_to_go = true;
int lastPosition;
int canvasW = 128;
int canvasH = 128;
TestObserver testObserver;



void setup() {
  size(128, 128, P2D);
  registry = new DeviceRegistry();
  testObserver = new TestObserver();
  registry.addObserver(testObserver);
  background(0);
  //client = new SyphonClient(this, "Modul8", "Main View");
  client = new SyphonClient(this);
}




void draw() {
  background(0);
  if (client.available()) {
  //if (client.newFrame()) {
    canvas = client.getGraphics(canvas);
    image(canvas, 0, 0, width, height);
    scrape();
  }  
  //scrape();
}

void stop()
{
  super.stop();
}

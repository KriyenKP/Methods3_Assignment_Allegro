/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

//This program is a box2D mesh with allegro...we're not necessarily gonna use it but I put it in anyway

#include <Box2D/Box2D.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <cstdio>

const float32 SCALE(50.0f);
const int HEIGHT(480);
const int WIDTH(640);
// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{
  B2_NOT_USED(argc);
  B2_NOT_USED(argv);

  // Define the gravity vector.
  b2Vec2 gravity(0.0f, 10.0f);

  // Do we want to let bodies sleep?
  bool doSleep = true;

  // Construct a world object, which will hold and simulate the rigid bodies.
  //b2World world(gravity, doSleep);

  b2World *world = new b2World(gravity);
  world->SetAllowSleeping(doSleep);


  // Define the ground body.
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(200.0f / SCALE, 420.0f / SCALE);

  // Call the body factory which allocates memory for the ground body
  // from a pool and creates the ground box shape (also from a pool).
  // The body is also added to the world.
  b2Body* groundBody = world->CreateBody(&groundBodyDef);

  // Define the ground box shape.
  b2PolygonShape groundBox;

  // The extents are the half-widths of the box.
  groundBox.SetAsBox(50.0f / SCALE, 10.0f / SCALE);

  // Add the ground fixture to the ground body.
  groundBody->CreateFixture(&groundBox, 0.0f);

  // Define the dynamic body. We set its position and call the body factory.
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(255.5f / SCALE, 350.0f / SCALE);
  b2Body* body = world->CreateBody(&bodyDef);

  // Define another box shape for our dynamic body.
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(15.0f / SCALE, 15.0f / SCALE);

  // Define the dynamic body fixture.
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;

  // Set the box density to be non-zero, so it will be dynamic.
  fixtureDef.density = 1.0f;

  // Override the default friction.
  fixtureDef.friction = 0.3f;

  //fixtureDef.restitution = 0.4f;
  // Add the shape to the body.
  body->CreateFixture(&fixtureDef);

  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float32 timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;


  al_init();
  al_init_image_addon();
  ALLEGRO_DISPLAY *display(al_create_display(WIDTH, HEIGHT));
  //ALLEGRO_BITMAP *ground(al_create_bitmap((50.0f / SCALE) * 2 * SCALE, (10.0f / SCALE) * 2 * SCALE));
  ALLEGRO_BITMAP *ground(al_create_bitmap(100, 20));
  al_set_target_bitmap(ground);
  al_clear_to_color(al_map_rgb(255,255,255));
  //ALLEGRO_BITMAP *box(al_create_bitmap((10.0f / SCALE) * 2 * SCALE, (10.0f / SCALE) * 2 * SCALE));
  ALLEGRO_BITMAP *fallingBox(al_create_bitmap(30, 30));

  al_set_target_bitmap(fallingBox);
  al_clear_to_color(al_map_rgb(255, 0, 255));
  al_set_target_bitmap(al_get_backbuffer(display));
  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();
  // This is our little game loop.
  for (int32 i = 0; true; ++i)
  {
    al_clear_to_color(al_map_rgb(0,0,0));
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world->Step(timeStep, velocityIterations, positionIterations);

    // Clear applied body forces. We didn't apply any forces, but you
    // should know about this function.
    world->ClearForces();

    // Now print the position and angle of the body.
    b2Vec2 position = body->GetPosition();
    b2Vec2 groundPos = groundBody->GetPosition();
    float32 angle = body->GetAngle();

    al_draw_bitmap(ground, (groundPos.x * SCALE)  - (al_get_bitmap_width(ground) / 2.0f), (groundPos.y * SCALE) - (al_get_bitmap_height(ground) / 2.0f), 0);
    //al_draw_bitmap(box, position.x * SCALE , position.y * SCALE - 20, 0);
    al_draw_bitmap(fallingBox, (position.x * SCALE) - (al_get_bitmap_width(fallingBox) / 2.0f), (position.y * SCALE) - (al_get_bitmap_height(fallingBox) / 2.0f), 0);
    al_flip_display();
    al_rest(1.0 / 5.0);
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
  }

  // When the world destructor is called, all bodies and joints are freed. This can
  // create orphaned pointers, so be careful about your world management.

  return 0;
}
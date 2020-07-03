byte neighbors = 0;
byte state = 0;
bool reloading = false;
Timer cooldown;
Timer reload;
byte clockCount;

void setup() {
  // put your setup code here, to run once:
  clockCount = 6;
}

void loop() 
  {
    neighbors = 0;

  switch (state)
  {
    case 0://Tile is ready to be fired.
    setColor(GREEN);
    FOREACH_FACE(f)
    {
      if (!isValueReceivedOnFaceExpired(f))
      {
        if (getLastValueReceivedOnFace(f) == 1 && reload.isExpired())
        {
          Shoot();
        }
      }
    }
    break;

    case 1://Tile is being fired.
    setColor(RED);
    if (cooldown.isExpired())
    {
      state = 2;
    }
    break;

    case 2://Tile is reloading.
    setColor(YELLOW);
    if (reload.isExpired())
    {
      state = 0;
    }
    break;

    case 3://Tile is a clock/sensor.
    setColor(WHITE);
    if (cooldown.isExpired() && clockCount > 0)
    {
      clockCount--;
      cooldown.set(2000);
    }

    if (clockCount <= 0)
    {
      //Wait for shot.
      setColor(MAGENTA);
      if (buttonDoubleClicked()) clockCount = 6; //Reset clock.
    }
    
    FOREACH_FACE(f)
    {
      if (f >= clockCount) setColorOnFace(dim(WHITE, 100), f);
    
      if (getLastValueReceivedOnFace(f) == 1)
      {
        if (clockCount > 0)
        {
          setColor(OFF);
          setColorOnFace(RED, f);
        } else
        {
          setColor(OFF);
          setColorOnFace(GREEN, f);
        }

        state = 4;
        reload.set(4000);
      }
    }


    break;

    case 4:
    {
      if (reload.isExpired())
      {
        clockCount = 6;
        state = 3;
      }
    }
  }

    
    // put your main code here, to run repeatedly:
    /*FOREACH_FACE(f) 
    {//check every face
      if (!isValueReceivedOnFaceExpired(f)) 
      {//someone there
        neighbors++;

        if (getLastValueReceivedOnFace(f) == 1 && state == 0 && !reloading)
        {
          Shoot();
        }
        
      }  
    }

    if (state == 3)
    {
      if (cooldown.isExpired())
      {
        if (clockCount == -1)
        {
          reload.set(8000);
          clockCount--;  
        }
        else
        {
          cooldown.set(4000);
          clockCount--;
        }
      }

      if (reload.isExpired())
      {
        reload.never();
        clockCount = 5;
        cooldown.set(4000);
        setColor(WHITE);
      }

      setColorOnFace(RED, clockCount);
    }
*/
    if (buttonReleased() && state == 0)
    {
      Shoot();
    }

    if (buttonLongPressed())
    {
      state = 3;
    }
    
    setValueSentOnAllFaces(state);
  }

void Shoot()
{
  state = 1;
  cooldown.set(100);
  reload.set(400);
}

void Reset()
{
  state = 0;
  reloading = false;
  clockCount = 5;
}

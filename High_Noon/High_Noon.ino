byte neighbors = 0;
byte state = 0;
bool reloading = false;
Timer cooldown;
Timer reload;
byte clockCount;
bool facesLit[6] = {false, false, false, false, false, false};
Timer startDelay;

void setup() {
  // put your setup code here, to run once:
  clockCount = 6;
  setColor(BLUE);
  startDelay.set(100);
  state = 0;/*
    for(byte count = 0; count != 6; count++)
    {
      facesLit[count] = 0;
    }*/
}

void loop() 
  {
    neighbors = 0;
  if (startDelay.isExpired())
  {
    switch (state)
    {
      case 0://Tile is ready to be fired.
      for(byte face = 0; face != 6; face++)
      {
        if (!isValueReceivedOnFaceExpired(face)) //If you are connected on the face.
        {
          facesLit[face] = true; //Light that specific face.
          if (getLastValueReceivedOnFace(face) == 1 && reload.isExpired()) //If that face is telling you to shoot.
          {
            Shoot();  
          }
        }
        if (facesLit[face] == true) setColorOnFace(YELLOW, face); //Light all the faces that are set to true.
        else setColorOnFace(BLUE, face); 
      }
      break;
  
      case 1://Tile is being fired.
      setColor(dim(RED, 64));
      FOREACH_FACE(f)
      {
        if (!isValueReceivedOnFaceExpired(f))
        {
          setColorOnFace(RED, f);
        }
      }
      
      if (cooldown.isExpired())
      {
        state = 2;
      }
      break;
  
      case 2://Tile is reloading.
      FOREACH_FACE(f)
      {
        if (!isValueReceivedOnFaceExpired(f))
        {
          if (getLastValueReceivedOnFace(f) == 5 || getLastValueReceivedOnFace(f) == 6)
          {
            state = getLastValueReceivedOnFace(f);
            reload.set(1000);
            break;
          }
        }
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
            setValueSentOnFace(5, f);
            setValueSentOnFace(6, (f+3)%6);
          } else
          {
            setColor(OFF);
            setColorOnFace(GREEN, f);
            setValueSentOnFace(6, f);
            setValueSentOnFace(5, (f+3)%6);
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
      break;
  
      case 5: //Misfired
      {
        FOREACH_FACE(f)
        {
          if (!isValueReceivedOnFaceExpired(f))
          {
            setColorOnFace(RED, f);
            setColorOnFace(RED, (f+3)%6);
          }
        }
        if (reload.isExpired())
        {
          state = 0;
        }
      }
      break;
  
      case 6: //Won
      {
        FOREACH_FACE(f)
        {
          if (!isValueReceivedOnFaceExpired(f))
          {
            setColorOnFace(GREEN, f);
            setColorOnFace(GREEN, (f+3)%6);
          }
        }
        if (reload.isExpired())
        {
          state = 0;
        }
      }
      break;
    }
  
      if (buttonReleased() && state == 0)
      {
        bool loaded = true;
        for(byte faces = 0; faces != 6; faces++)
        {
          if (!facesLit[faces]) 
          {
            loaded = false;
            break;
          }
        }
        if (loaded) Shoot();
      }
  
      if (buttonLongPressed())
      {
        if (state != 0) state = 0;
        else state = 3;
      }
  
      if (state != 4) setValueSentOnAllFaces(state);
    }
  }
void Shoot()
{
  state = 1;
  cooldown.set(100);
  reload.set(400);

  for(byte count = 0; count != COUNT_OF(facesLit); count++)
  {
    facesLit[count] = false;
  }
}

void Reset()
{
  for(byte count = 0; count != COUNT_OF(facesLit); count++)
  {
    facesLit[count] = false;
  }
  
  state = 0;
  reloading = false;
  clockCount = 5;
}

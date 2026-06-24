# Gaming Campus Light Engine

# Table of Contents
1. [gcle](#gcle)     
2. [Engine](#engine)    
3. [Render](#render)    
4. [Core](#core)    


## gcle


## Engine

---
## Render

You normaly don't need to use this part of the code. It's the render library where all SDL2 code is used.       
The only one you will needs are:

a. [Sound](#sound)     
b. [Music](#music)      
c. [Audio](#audio)     
d. [Text](#text) 

---
### Sound:

#### InitSound(Path)

Initialize the sound you'll want to play.

---
#### PlaySound(Mode, Volume)

Play the actual loaded sound.   
Mode define how much time you want to loop on the sound.   
If you want to loop infinitely this parameter should be -1.

---
### Music

#### InitMusic(Path)

Initialize the music you'll want to play.

---
#### PlayMusic(Mode)

Play the actual loaded music.   
Mode define how much time you want to loop on the music.   
If you want to loop infinitely this parameter should be -1.

---
### Audio

All next functions can be called by writing Audio::Funtion() and are explicit.

#### SetMusicVolume(Volume)
#### StopMusic()
#### PauseMusic()
#### ResumeMusic()
#### StopAllSound()
#### IsAMusicPlaying()
#### IsAMusicPaused()

---
### Text:

#### Usage

In your scene use the function CreateText("text", x, y, width , height)

```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
```

---
#### SetText("text")

Replace the actual text by the new one.
```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
pText->SetText("Hello World!")
```

return ``` Hello World! ```

---
#### SetColor(r, g, b ,a)

Replace the actual text color by the new one.

```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
pText->SetColor(255, 0, 0, 255)
```

return <span style="color:red">Test</span>

---
#### SetFont("font")

Replace the actual font by the new one.
Can be .ttf or .otf

```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
pText->SetFont("Papyrus.ttf")
```

return <span style="font-family:Papyrus;">Test</span>

---
#### SetPosition(x, y)

Replace the actual position by the new one.

---
#### SetWidth(width)

```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
pText->SetWidth(30)
```

return <span style="display:inline-block; transform:scaleX(1.5);">   Test</span>

---
#### SetHeight(height)

```cpp
std::string text = "Test";
Text* pText = CreateText(text, 40, 40, 20, 20);
pText->SetHeight(30)
```

return <span style="display:inline-block; transform:scaleY(1.5);">   Test</span>

---
## Core
a. [Vector2](#vector2)     
b. [Utils](#utils)      
c. [MathGC](#mathgc)     
d. [InputManager](#inputmanager)    

---
### Vector2:
  
Vector2f = $Vector2<float>$        
Vector2u = $Vector2<int> $

---
#### Constructor
Vector2() = {x = 0, y = 0}      
Vector2(int i) = {x = i, y = i}      
Vector2({4, 5}) = {x = 4, y = 5}        
Vector2(Vector2() u) = {x = u.x, y = u.y}    

---
#### NormSquared

return ${x * x + y * y}$

---
#### Norm

return $\sqrt{{x * x + y * y}}$ 

---
#### Dot (Vector2 other)

return ${x * other.x + y * other.y}$

---
#### Angle (Vector2 other)

return $acos(Dot(other) / Norm())$

---
#### Normalized

return ${(x, y) / Norm()}$

---
#### GetDistance (Vector2 other)
$x2 = other.x - x$     
$y2 = other.y - y$    
     
return ${x2 * x2 + y2 * y2}$

---

### Utils:  
---
#### DEBUG_INFO:
Use: DEBUG_INFO << "Message" << ENDL  
Output: "[INFO]: Message"  

---
#### DEBUG_WARN:
Use: DEBUG_WARN << "Message" << ENDL  
Output: "[WARN]: Message"  

---
#### DEBUG_ERROR:
Use: DEBUG_ERROR << "Message" << ENDL  
Output: "[ERROR]: Message"  

---
### assert(condition)
Use:  
int foo = 2;  
assert(foo == 3)

Output: Exception throw  

---
Use:  
int foo = 2;  
assert(foo == 2)

Output: Nothing

---

### Profiler:
#### PROFILER_START(ID, Name)
#### PROFILER_END(ID)

Use:  
PROFILER_START("sleep", "Sleep Duration");  
Sleep(3000);  
PROFILER_END("sleep);  

Output: [INFO]: Your task : Sleep Duration took 3000764000 nanoseconds | 3.00076 seconds

---

### MathGC:
---

#### MathGC::RadToDeg(Radians rad)

return **Radians in Degrees**

---
#### MathGC::DegToRad(Degrees deg)

return **Degrees in Radians**

---
#### Lerp($minValue, maxValue, time$)

return $(time - 1) * minValue + time * maxValue$

---

### InputManager:

How to use ?    

```cpp

// Can be:
//  IsDown
//  IsHeld
//  IsUp

if (InputManager::GetInstance().IsDown('KeyCode'))
{
    //do something
}
```
---



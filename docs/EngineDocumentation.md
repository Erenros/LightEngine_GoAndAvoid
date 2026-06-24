# Gaming Campus Light Engine

# Table of Contents
1. [gcle](#gcle)     
2. [Engine](#engine)    
3. [Render](#render)    
4. [Core](#core)    

---

## gcle

---

## Engine

This is where you'll find most of the functions you want to use.

### Entity

- [Destroy](#destroy)
- [ToDestroy](#todestroy)

- [GetShape](#getshape)
- [GetPosition](#getpositionoffsetx--05f-offsety--05f)
- [GetId](#getid)

- [SetTag](#settag)
- [IsTag](#istag)

- [GoToPosition](#gotoposition)
- [GoToDirection](#gotodirection)
- [SetSpeed](#setspeed)
- [SetDirection](#setdirection)

- [SetPosition](#setposition)
- [SetScale](#setscale)
- [ScaleBy](#scaleby)
- [GetScale](#getscale)

- [GetRotation](#getrotation)
- [SetRotation](#setrotation)
- [Rotate](#rotate)

- [GetRigidBody](#getrigidbody)
- [SetRigidBody](#setrigidbody)
- [IsRigidBody](#isrigidbody)

- [SetTexture](#settexture)
- [AddAnimation](#addanimation)
- [PlayAnimation](#playanimation)

- [IsColliding](#iscolliding)
- [IsInside](#isinside)

---

## Destroy()

Destroy the Entity at the end of the frame.

---

## ToDestroy()

Returns whether the Entity is scheduled to be destroyed at the end of the frame.

---

## GetShape()

Returns the `gcle::Shape*` of the Entity.

---

## GetPosition(offsetX = 0.5f, offsetY = 0.5f)

Returns the actual position of the Entity with an offset.

- `offsetX = 0.5f`, `offsetY = 0.5f` → center of the shape  
- `offsetX = 0.0f`, `offsetY = 0.0f` → top-left of the shape  

---

## GetId()

Each created Entity has a different ID based on its creation order.

---

## SetTag(tag)

Sets a tag to the entity, which can be used with `IsTag`.

---

## IsTag(tag)

Returns `true` if the given tag matches the Entity’s tag.

---

## GoToPosition(x, y, speed = -1.0f)

Makes the Entity move to the given position.

---

## GoToDirection(x, y, speed = 1.0f)

Makes the Entity move in the given direction.

---

## SetSpeed(speed)

Sets the Entity’s speed.

---

## SetDirection(x, y)

Sets the Entity’s direction.

---

## SetPosition(x, y, offsetX = 0.5f, offsetY = 0.5f)

Sets the position of the Entity using an offset between 0 and 1.

---

## SetScale(Vector2f) | SetScale(Scale)

Sets the scale of the Entity shape. Can use a Vector2f to scale in x and/or y.

---

## GetScale()

Returns the scale of the Entity shape.

---

## ScaleBy(Vector2)

Scales the Entity shape by the given parameter.

---

## SetRotation(angle)

Sets the rotation of the Entity shape (in degrees).

---

## GetRotation()

Returns the rotation of the Entity shape in degrees.

---

## Rotate(angle)

Rotates the Entity shape by the given parameter.

---

# Render

You normally don’t need to use this part of the code. It is the rendering library where all SDL2 code is used.

### Sound

## InitSound(path)

Initializes the sound you want to play.

---

## PlaySound(mode, volume)

Plays the currently loaded sound.

- `mode` defines the number of loops  
- `-1` = infinite loop  

---

### Music

## InitMusic(path)

Initializes the music you want to play.

---

## PlayMusic(mode)

Plays the currently loaded music.

- `mode` defines the number of loops  
- `-1` = infinite loop  

---

### Audio

All functions can be called using `Audio::Function()`.

- SetMusicVolume(volume)  
- StopMusic()  
- PauseMusic()  
- ResumeMusic()  
- StopAllSound()  
- IsMusicPlaying()  
- IsMusicPaused()  

---

### Text

### Usage

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



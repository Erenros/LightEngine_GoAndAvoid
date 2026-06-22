# Gaming Campus Light Engine

# Table of Contents
1. [gcle](#gcle)
2. [Engine](#engine)
3. [Render](#render)
4. [Core](#core)


## gcle


## Engine


## Render


## Core
a. [Vector2](#vector2)
b. [Utils](#utils)
c. [MathGC](#mathgc)
d. [InputManager](#inputmanager)

### Vector2:

---
#### Defined Vector2 types
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



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
To use all the next feature given by the core lib you have to #include "includes.h

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







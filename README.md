# Python-ComputeShader
OpenGL 4.3 required

# Requirements

> Windows platform<br>
> Python 3.5<br>

# Functions

NewCS
```
cs = NewCS(source)
```

----------------------------------------

UseCS
```
UseCS(cs, x, y, z)
```
x, y, z are optional
default values are x, y, z = 1, 1, 1

----------------------------------------

DeleteCS
```
DeleteCS(cs)
```

----------------------------------------

NewSSBO
```
ssbo = NewSSBO(content)
```

----------------------------------------

UseSSBO
```
UseSSBO(ssbo, cs, name)
```

----------------------------------------

UpdateSSBO
```
UpdateSSBO(ssbo, data, offset)
```
offset is optional
default value for offset is 0

----------------------------------------

ReadSSBO
```
data = ReadSSBO(ssbo, size, offset)
```
offset is optional
default value for offset is 0

----------------------------------------

DeleteSSBO
```
DeleteSSBO(ssbo)
```

----------------------------------------

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

UseCS
```
UseCS(cs, x, y, z)
```

DeleteCS
```
DeleteCS(cs)
```

NewSSBO
```
ssbo = NewSSBO(content)
```

UseSSBO
```
UseSSBO(ssbo, cs, name)
```

UpdateSSBO
```
UpdateSSBO(ssbo, data, offset)
```

ReadSSBO
```
data = ReadSSBO(ssbo, size, offset)
```

DeleteSSBO
```
DeleteSSBO(ssbo)
```

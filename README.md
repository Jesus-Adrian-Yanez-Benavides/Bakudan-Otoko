# Bomberman II - 1v1

Un juego competitivo de Bomberman para dos jugadores donde solo uno puede sobrevivir.

## 🎮 Controles

### Jugador 1 (Esquina Superior Izquierda)
- **Movimiento:** Flechas del teclado (↑ ↓ ← →)
- **Bomba:** ESPACIO

### Jugador 2 (Esquina Inferior Derecha)
- **Movimiento:** WASD
- **Bomba:** CTRL Izquierdo

## 🎯 Objetivo

Sé el último jugador vivo:
- Coloca bombas para destruir bloques y eliminar a tu oponente
- Evita las explosiones
- El jugador que sobreviva gana

## 📋 Características del Juego

✅ **Dos Jugadores Simultáneos:** Controles independientes para cada jugador  
✅ **Sistema de Explosiones:** Las bombas explotan en forma de cruz (4 direcciones)  
✅ **Destrucción de Bloques:** Las explosiones destruyen bloques suaves pero no las paredes duras  
✅ **Detección de Colisiones:** Los jugadores no pueden pasar a través de bombas ni paredes  
✅ **Pantalla de Victoria:** Se muestra quién es el ganador y permite salir del juego  
✅ **Mapa Procedural:** Diseño clásico de Bomberman con 15x13 tiles  

## 🏗️ Estructura del Proyecto

```
Bakudan-Otoko/
├── src/
│   ├── main.cpp         # Bucle principal del juego
│   ├── Player.cpp       # Lógica de jugadores
│   ├── Bomb.cpp         # Sistema de bombas
│   ├── Explosion.cpp    # Sistema de explosiones
│   └── Map.cpp          # Lógica del mapa
├── include/
│   ├── Player.h
│   ├── Bomb.h
│   ├── Explosion.h
│   └── Map.h
├── assets/
│   ├── Tiles.png        # Tileset del juego
│   └── NES-Bomberman-II-Miscellaneous-Bomberman-_-Enemies.png
├── bin/
│   └── Bomberman.exe    # Ejecutable compilado
└── .vscode/
    └── tasks.json       # Configuración de compilación
```

## 🔧 Compilación

El proyecto utiliza **SFML** para gráficos. Para compilar:

```bash
g++ -fdiagnostics-color=always -g src/*.cpp -I./include -o bin/Bomberman.exe -lsfml-graphics -lsfml-window -lsfml-system
```

Desde VS Code, usa: `Ctrl+Shift+B` (o ejecuta la tarea de compilación)

## 🎬 Ejecución

```bash
./bin/Bomberman.exe
```

## 🕹️ Cómo Jugar

1. **Jugador 1** comienza en la esquina superior izquierda (posición 1,1)
2. **Jugador 2** comienza en la esquina inferior derecha (posición 13,11)
3. Ambos jugadores pueden:
   - Moverse por el mapa libremente (excepto bloques)
   - Colocar bombas con su tecla asignada
4. Las bombas **explotan después de 3 segundos**
5. La explosión se expande hasta 2 tiles en cada dirección (arriba, abajo, izquierda, derecha)
6. Un jugador que toca una explosión **muere**
7. **Gana** el último jugador vivo

## 🎨 Parámetros Personalizables

En el código encontrarás constantes que puedes modificar:

- `timeToExplode` en `Bomb.cpp`: Tiempo de detonación (por defecto: 3 segundos)
- `explosionRange` en `Explosion.cpp`: Rango de expansión (por defecto: 2 tiles)
- `speed` en `Player.cpp`: Velocidad de movimiento (por defecto: 4.0f pixels/frame)

## 📝 Tecnologías

- **C++17**: Lenguaje de programación
- **SFML 2.5+**: Librería gráfica para C++
- **GCC MinGW64**: Compilador

## 🐛 Notas

- Si no se carga el archivo `font.ttf`, la pantalla de victoria no mostrará texto, pero el juego seguirá funcionando normalmente
- Las imágenes de sprites deben estar en la carpeta `assets/`
- El juego corre a 60 FPS

## 📄 Licencia

Proyecto educativo basado en el clásico Bomberman de NES.

---

**¡Que gane el mejor!** 💣

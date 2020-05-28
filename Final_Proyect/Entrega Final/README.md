# Trabajo Final

Primeramente me gustaría recalcar que es juego está hecho desde un punto de vista cómico. La situación actual es muy seria pero, no está hecho ni con malicia, ni desde un punto de vista político. Simplemente me pareció una idea divertida para el juego toda la temática de Fernando Simón escapando del coronavirus.

El juego tarde alrededor de 40 segundos en cargar al inicio debido a todos los objetos y texturas que utilizo.

#### Controles

* ASWD - Movimiento

* Mover Ratón - Mirar hacia todos los lados (cámara en primera persona)

* Boton izquierdo del ratón - Disparar

* Botón derecho del ratón - Apuntar

* Número 1 - Modo de día

* Número 2 - Modo noche

### Características

Se ha utilizado la versión 3.3 de OpenGL.

He utilizado Assimp para importar los modelos, utilizando una clase Model y una clase Mesh, los modelos los he editado utilizando Blender, he importado algún modelo de internet como la pistola o la casa y he juntado varios meshes en un solo modelo como las diferentes partes de la isla. Casi todos los assets del juego utilizan mapas normales, además de mapas especulares y las propias texturas. A excepción, de las palmeras. La conversión se hace desde el fragment shader pasándole la matriz de transformación del tangent space al world space. Así por ejemplo si no le paso mapas normales puedo no usar la matriz de cambio de base para usar la propia normal de los objetos. Otras figuras más simples del juego como el skybox o el minimapa que son cubos y cuadrados los creo de la forma vista en clase.

###### Transformaciones

Toda la animación de la pistola está hecha 'a pelo' transladando, por ejemplo, el mesh del tambor de la pistola al centro girándolo y posteriormente devolviéndolo a su posición para así conseguir una animación. Todas las partes de la pistola adquieren la matriz del modelo del cuerpo de la misma.

###### Cámara

Se utiliza una cámara en primera persona utilizando coordenadas esféricas y tres vectores directores el frente, derecha y arriba se restringe el movimiento en el eje Y para no poder ir hacia arriba o hacia abajo. La perspectiva de la cámara cambia el fovy dependiendo si apuntas o no. Existe otra cámara que mira por encima del jugador utilizando la proyección ortogonal para el minimapa. Además se utilizan las coordenadas de la matriz del Lookat elimiando las translaciones para poder alinear la pistola y las balas con la posición a donde apunta la cámara y siempre se vean igual al mover la cámara.

###### Luces

Hay un modo dia donde la luz es direccional y cuyo vector trata de replicar la dirección del sol del skybox. Existe también, un modo noche donde la luz proviene de un spotlight que viene de una linterna que solo se pinta en modo noche. En este modo se elimina el skybox y queda el fondo negro para conseguir un efecto más de miedo ya que todos los skybox nocturnos que probé, no quedaban tan bien. Para el spotlight se utiliza un cutoff interior y exterior para conseguir un efecto más realista y que el foco se vaya difuminando un poco en el exterior.

###### Otros elementos a tener en cuenta

* Si un coronavirus se acerca mucho se pinta un rectángulo rojo que va aumentando la componente alpha según se acerca para avisar al jugador de que está en peligro.

* Se utiliza un framebuffer para renderizar la escena desde arriba y se pega en un rectángulo en la parte superior derecha de la pantalla para crear el minimapa.

* Se utiliza la biblioteca Freetype para imprimir el número de rondas y los coronavirus activos en cada momento.

* Cree una texturas con un editor online para los estados de la partida tras perder, ganar, inicio de ronda. Estas texturas son .png y en los casos donde su componente alpha sea muy bajo se descarta el fragmento.

* Para la detección de colisiones se han utilizado esferas debido a que los coronavirus eran esféricos y me parecía que se adecuaba más que los rectángulos para este caso.

* Para el modo día se utiliza un Skybox utilizando un cubemap.

* En el modo noche se incluye una linterna al lado de la pistola

* Se han añadido diferentes sonidos como música al inicio, sonido al disparar y al cambiar de bala, sonido al impactar la bala, sonido ambiente, al moverse (sonido de pisadas), al inicio de cada ronda y cuando se acerca mucho un coronavirus. Así como al ganar y perder. Se ha utilizado el motor de sonido Irrklang.

# ARCADE


Documento para Proyecto 1; Arcade

List Members: Abel Nuñez; Alejandro Rodriguez; Hugo Escobar.

Abel Nuñez: https://github.com/abelnunez5

Alejandro Rodiguez: https://github.com/Alejandrorg5

Hugo Escobar: https://github.com/HuggoESC

Este proyecto es una recreación del primer nivel del clásico Super Mario Bros de la NES, desarrollado en C++ por un equipo de tres entusiastas del desarrollo de videojuegos.
La idea es capturar la esencia retro del juego original, respetando su estética, mecánicas y feeling, pero aprovechando lo que hemos aprendido de programación moderna.
Somos un grupo de tres estudiantes/desarrolladores apasionados por los videojuegos clásicos y el desarrollo en C++. 
Este proyecto es tanto un desafío técnico como un homenaje a uno de los títulos más icónicos de la historia.

¡Bienvenido a la aventura clásica de Mario, recreada con amor usando Raylib!

Guía a Mario a través del mundo 1-1, esquivando enemigos, saltando sobre bloques, recolectando monedas y evitando obstáculos hasta llegar al final del nivel. ¡Tu meta es sobrevivir, sumar puntos y completar el mundo antes de que se acabe el tiempo!

CONTROLES:

← / →: Mover a Mario a la izquierda o derecha.

ESPACIO: Saltar.

↑ / ↓ (en menú inicial): Elegir entre "1 jugador" o "2 jugadores" (solo visual, el código actual usa 1 jugador).

ENTER: Confirmar selección y comenzar la partida.

- El juego se desarrolla en un escenario horizontal con plataformas, bloques, tuberías y enemigos como los famosos Goombas.

- Mario puede caminar, saltar, golpear bloques desde abajo y recoger monedas.

- Existen bloques especiales (amarillos) que pueden contener sorpresas.

- Si Mario cae al vacío o pierde todas sus vidas, se termina la partida.

PANTALLAS:

- Pantalla de inicio: Elige entre 1 o 2 jugadores (solo se implementa 1 por ahora).

- Intro: Una pantalla de presentación que muestra tu puntaje, mundo actual y número de vidas.

- Partida activa: El juego comienza, y tú controlas a Mario.

TIPS:

- ¡Mantente atento al tiempo! Tienes un límite para completar el nivel.

- Mario puede saltar sobre los enemigos para eliminarlos.

- Recolecta monedas para aumentar tu puntaje.

- Explora los bloques amarillos para encontrar potenciadores o sorpresas ocultas.



FEATURES IMPLEMENTED: 





Link SUper mario: https://www.youtube.com/watch?v=7D4uoSoQsjw 

Link sprites: https://www.spriters-resource.com/nes/supermariobros/

Link to the project: https://github.com/HuggoESC/ARCADE.git

Link to the Video Project: https://www.youtube.com/watch?v=wtYIUS55WBo

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Super Mario Bros Remake - UPC</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');

    body {
      font-family: 'Press Start 2P', cursive;
      background-color: #1a1a1a;
      color: #fce803;
      margin: 0;
      padding: 20px;
      background-image: url('https://images.steamusercontent.com/ugc/794263213710474663/56FC1D9E69A18E93EEF13E56DB5D9718FE046382/?imw=512&&ima=fit&impolicy=Letterbox&imcolor=%23000000&letterbox=false');
      background-size: cover;
      background-repeat: no-repeat;
      background-attachment: fixed;
    }

    header {
      background-color: #e63946;
      color: white;
      padding: 20px;
      text-align: center;
      border-radius: 10px;
      border: 4px solid #fff;
      box-shadow: 0px 0px 15px #fce803;
    }

    section {
      background: rgba(0, 0, 0, 0.85);
      padding: 20px;
      margin-top: 20px;
      border-radius: 10px;
      border: 2px solid #fce803;
      box-shadow: 0px 0px 10px #fce803;
    }

    h1, h2 {
      color: #ffffff;
    }

    ul {
      list-style-type: square;
      padding-left: 20px;
    }

    a {
      color: #00ffff;
      text-decoration: none;
    }

    a:hover {
      text-decoration: underline;
    }

    iframe {
      width: 100%;
      max-width: 560px;
      height: 315px;
      display: block;
      margin: 0 auto;
      border: 4px solid #fce803;
    }
  </style>
</head>
<body>
  <header>
    <h1>Super Mario Bros (NES) Remake</h1>
    <p><strong>TEAM UNICEF - UPC 2025</strong></p>
  </header>

  <section>
    <h2>Bienvenido</h2>
    <p>Este proyecto es un tributo al icónico <strong>Super Mario Bros</strong> (1985), recreado fielmente como parte del curso universitario en la <strong>UPC</strong>, utilizando C++ y la librería Raylib.</p>
    <p><em>🎮 Proyecto académico sin fines comerciales.</em></p>
  </section>

  <section>
    <h2>Resumen del Juego</h2>
    <p>Recreación completa del nivel 1-1 del juego original con:</p>
    <ul>
      <li>Movimiento y físicas auténticas</li>
      <li>Enemigos: Goombas y Koopas</li>
      <li>Ítems: Champiñones, monedas</li>
      <li>Música y efectos de sonido</li>
      <li>Herramientas de depuración: colliders, áreas visibles</li>
    </ul>
    <p><strong>Ganas:</strong> al llegar a la bandera e ingresar al castillo.<br>
       <strong>Pierdes:</strong> si caes o te quedas sin vidas.</p>
  </section>

  <section>
    <h2>Contribuciones del Equipo</h2>
    <ul>
      <li><strong>Alejandro Rodríguez:</strong> Lógica del juego, power-ups, cámara y colisiones.</li>
      <li><strong>Abel Núñez:</strong> Animaciones, IA de enemigos, sistema de colisiones y físicas avanzadas.</li>
      <li><strong>Hugo Escobar:</strong> Música, efectos, estados de victoria/derrota y UI final.</li>
    </ul>
  </section>

  <section>
    <h2>Vídeo del Gameplay</h2>
    <iframe src="https://www.youtube.com/embed/YOUR_VIDEO_ID" allowfullscreen></iframe>
    <p>Incluye:</p>
    <ul>
      <li>Intro con logo de UPC y miembros del equipo</li>
      <li>Gameplay con subtítulos explicativos</li>
      <li>Mecánicas de victoria y derrota</li>
      <li>Interacción con enemigos e ítems</li>
      <li>Depuración en tiempo real</li>
    </ul>
  </section>

  <section>
    <h2>Descarga</h2>
    <p><a href="ENLACE_A_TU_ZIP">⬆️ Descargar Super Mario Bros Remake (.zip)</a></p>
    <p>Compatible con Windows. Extrae el archivo y ejecuta <code>SuperMario.exe</code>.<br>
    Recomendado: jugar con teclado o mando.</p>
  </section>
</body>
</html>

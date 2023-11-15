<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/42sin/cub3d">
    <img src="logo.png" alt="Logo" width="128" height="128">
  </a>

<h3 align="center">cub3D</h3>

  <p align="center">
    My first RayCaster with miniLibX
    <br />
    <a href="https://github.com/42sin/cub3D/issues">Report Bug</a>
    ·
    <a href="https://github.com/42sin/cub3D/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

#### [Go to Subject PDF]
[![Subject PDF][subjectImage]](en.cub3d_subject.pdf)

This is the second group project of the 42 core curriculum.<br>
It is inspired by the world-famous Wolfenstein 3D game and is an opportunity to explore the large world of ray-casting.
This project’s objectives are similar to all this first year’s objectives: Rigor, use of C, use of basic algorithms, information research etc.<br>
As a graphic design project, we had to use the 42 graphics library miniLibX.<br>
To conclude cub3D is a remarkable playground to explore the playful practical applications of mathematics.<br>
With the help of the numerous documents available on the internet, you will use mathematics as a tool to create elegant and efficient algorithms.
This project was done in collaboration with <a href="https://github.com/FlorianBindereif">Florian Bindereif</a>.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

To run the project, OpenGL and AppKit are required for the miniLibX library to work properly.<br>
If you want to run this program on Linux <a href="https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html#compilation-on-linux">here</a> is a tutorial that might help to install the library on Linux. This was not tested by us, so you may face problems.

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/42sin/cub3D.git && cd cub3D
   ```
2. Compile the project
   ```sh
   make
   ```
3. Run the program
   ```sh
   ./cub3D map.cub
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

- Move your camera with your arrow keys or by dragging with your mouse
- Move the character with `WASD`
- Interact with the environment by pressing `Enter`
<br>
Here is an example
<img src="example.gif">

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Features -->
## Features
You can change the windows size, movement speed, difficulty, minimap size, etc. inside of `srcs/includes/cub3d_defines.h`
### Gameplay
- minimap
- move the camera by leftclicking and dragging or by pressing the arrow buttons
- pressing `ESC` or clicking the cross of the window closes the program
- enemies
- wall collision
- game gets closed when an enemy touches you
- doors that can be opened/closed
- 5 different sprites (barrel, light, pillar, door, animated enemy)
### Map
Inside of the repository you are provided with an example map but feel free to change the map to your likings, as long as it is a valid map. You can also change the textures as well inside of `textures/`
- `0` define walkable spaces
- `1` define walls
- `D` define doors
- `F` defines where the enemy spawns
- `B` defines barrels
- `L` defines lights
- `P` defines pillars
- the players position is displayed by:
  - `N` player is facing north (top of the 2D map)
  - `E` player is facing east (right side of the 2D map)
  - `S` player is facing south (bottom of the 2D map)
  - `W` player us facing west (left side of the 2D map)
- the map has to be enclosed by walls on all sides
- set all the textures that should be used as wall texture i.e.
  - `WE images/1.xpm`
  - `EA images/2.xpm`
  - `SO images/3.xpm`
  - `NO images/4.xpm`
- set the ceiling color, i.e.:
  - `C 0, 183, 183`
- set the floor color, i.e.:
  - `F 141, 1, 126`

For more information about the specific requirements and tasks, check out the <a href="en.cub3d_subject.pdf"> Subject PDF</a>.


<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[issues-url]: https://github.com/42sin/cub3D/issues
[license-url]: https://github.com/42sin/cub3D/blob/master/LICENSE.txt
[subjectImage]: eval.png
[Go to Subject PDF]: en.cub3d_subject.pdf

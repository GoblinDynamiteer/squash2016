<h1>SQUASH SM 2017</h1>
<h2>Racket med LED-slinga</h2>  

---


**Elevprojekt för Squash-SM i Linköping 2017**   
*Mjukvaruutvecklare inbyggda system*  
*Mölk Utbildning, Mjärdevi - Linköping*  

---

Projektets mål är att utrusta två stycken squash-racket så att dessa efterliknar ljussablar.

En ljussabel/lasersvärd (eng. Lightsaber) är ett närstridsvapen från det kända, fiktiva Star Wars-universumet.

---
<h5>Projektmål:</h5>  

*Grundmål:*
- LED-slinga runt rackets ytterkant som ändrar intensitet vid bollträff.
- Uppspelning av ljud vid bollträff.
- Uppspelning av konstant ljud vid påslagen lightsaber.
- Inbyggd/gömd elektronik och batteridrivning

*Utökade mål:*
- Trådlös kommunikation för uppspelning av ljud med externa högtalare. Wifi/Bluetooth.
- Gyro/Accelerationssensor för uppspelning av modiferat ljud vid rörelse av racket.

*Projektslut 2017-03-17:*
- Ljudeffekter valdes att ej användas.
- Endast ett racket byggdes, pga förstörd hårdvara vid lödning
- Ett dutt-räkningsspel byggdes, utöver "lightsaber"-läget.
- En strömbrytare implementerades

*Erfarenheter:*
- Lödning och hårdvaru-arbete
- ICSP-programmering av Arduino-kort med AVRDUDESS GUI
- Enklare kodning

---

Ingående delar:
- Arduino NANO
- ~~Adafruit Audio FX Sound Board~~
- ~~Adafruit Mono 2.5W Class D Audio Amplifier~~
- Piezo Vibration Sensor
- Resistor 1 MegaOhm
- ~~Högtalare 8 ohm~~
- DotStar APA102 Digital LED Strip
- 2x Batterier LiPo 18650
- ON/OFF-knapp

---

**Kod**  
[hitCounter / Dutträknare](https://github.com/GoblinDynamiteer/squash2016/blob/master/Code/Code%20Main/hitCounter/hitCounter.ino)  
[Färgbyte vid spel](https://github.com/GoblinDynamiteer/squash2016/blob/master/Code/Code%20Main/lightsaber/lightsaber.ino)  

**Inspelningar Utveckling:**  
[Test av färdigt racket, spel-läge ](https://www.youtube.com/watch?v=IrT2sxNLXvo)<br>
[Test av färdigt racket, counter-läge](https://www.youtube.com/watch?v=nRJhmAD6vJA)  
[Testning av vibrationssensor](https://youtu.be/zGH6sVq6qng)

**Inspelningar Squash-SM 2017:**  
[Speltest 1](https://youtu.be/f3043sTv894)  
[Speltest 2](https://youtu.be/pRtZToY75Fc)  
[Speltest HitCounter 1](https://youtu.be/iNCcjWJ_8ho)  
[Speltest HitCounter 2](https://youtu.be/gF-8KtOZpgw)  

---

**Fotografier:**  
Slagtest med vibrationssensor och LED som indikator   
![Slagtest](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/vibration_test.png)

Ljud-prototyping med Arduino UNO, breadboard, Adafruit Audio FX och ljudförstärkare. Samt tejp.
![Frankenstein](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/frankenstein_racket.png)

Testmontering av LED-slinga, prov av ljusstyrka och olika färger.
![darkvslight](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/dark_vs_light_export.png)

Batterier av typen 18650, stulna från ett gammalt laptop-batteri.
![Power!](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/batteries.png)

Lödning av vibrationssensor  
![Smokez](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/soldering.png)

Alla lödningar klara  
![Pilljobb](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/soldering2.png)

Jordkablar
![Earth](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/cable_soldering.png)

Batterianslutning i handtaget
![PowerConn](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/battery_connectors.png)

Inbyggt system
![IS2016](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/mounted_hardware.png)

ICSP programming
![icp](https://raw.githubusercontent.com/GoblinDynamiteer/squash2016/master/Documentation/Photoshop/icsp.png)

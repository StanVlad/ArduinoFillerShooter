# ArduinoFillerShooter

Componente folosite:
1 x placa Arduino UNO,
1 x breadboard mare,
1 x breadboard mic,
1 x matrice led-uri,
1 x driver pt. matrice,
1 x joystick,
5 x fire tip mama-tata,
29 x fire tip tata-tata,
1 x rezistor 100k,
2 x condensator.

Scopul jocului:
Controleaza tancul si umple cat mai multe spatii goale din formele care se indreapta spre tine, pentru a acumula puncte si pentru a evita sa fii distrus. Fiecare rand umplut reprezinta un punct. Jocul se termina cand formele au ajuns la tancul tau.

Descriere:
Jocul se porneste odata cu conectarea la o sursa de curent. Dupa animatia de start, veti observa un ecran de start. La miscarea stanga-dreapta din joystick veti putea modifica nivelul de dificultate, afisat pe ultima linie a matricei de led-uri sub forma unei bare de dificultate, dupa cum urmeaza:
2 led-uri aprinse = usor;
3 led-uri aprinse = mediu;
4 led-uri aprinse =  greu.
Nivelul de dificultate se refera la numarul de spatii goale ce trebuie umplute. Cu cat mai mare dificultatea, cu atat mai multe spatii de umplut. In timp, viteza cu care formele avanseaza spre tine creste.
Dupa selectarea dificultatii se apasa pe butonul joystick pentru a incepe jocul.
Prin miscarea stanga-dreapta a joystick-ului, jucatorul controleaza tancul. Prin apasarea butonului joystick, jucatorul trage o bila. Bila se va deplasa vertical pe cat posibil. Generarea formelor se face periodic. Obtinerea de puncte castigatoare se realizeaza simultan cu umplerea de randuri, dupa cum scrie mai sus. O data ce este umplut un rand, acesta va fi ars, tragand inapoi in sus randurile de sub el.
Cand jucatorul va pierde, scorul se va afisa pe matrice timp de 5 secunde, dupa care matricea se reseteaza si revine la meniul initial.

Distractie placuta!

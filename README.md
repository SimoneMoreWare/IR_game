# IR_game
Arduino: creare un gioco con ricevitore IR
In questo articolo si vedrà in che modo realizzare un gioco interattivo con Arduino utilizzando un telecomando e un ricevitore IR.

Non vi sono restrizioni sul telecomando da usare, in quanto ad ogni avvio sarà possibile mappare i tasti in qualsiasi modo si desidera.

Il gioco consiste nel premere i tasti associati al simbolo che compare sul display LCD. Man mano che si sale di livello il gioco sarà più difficile in quanto i simboli si muoveranno più velocemente. Il punteggio sale nel momento in cui si premono i tasti giusti, altrimenti diminuisce,

Per realizzare tale progetto occorre utilizzare i seguenti componenti:

1x board di Arduino Uno
1x display LCD
1x potenziometro da 10k
1x buzzer passivo
1x ricevitore IR
1x telecomando
vari jumper
Ecco un video che illustra il progetto: https://www.youtube.com/watch?v=hH3VIro2u5Y&feature=emb_imp_woyt

Il diagramma di collegamento è il seguente:

![alt text](https://i0.wp.com/www.moreware.org/wp/wp-content/uploads/2020/04/Untitled-Sketch-2_bb.png?resize=768%2C619&ssl=1)

In alto a sinistra dello schermo è presente il livello, in alto a destra il punteggio. Se il punteggio è minore di zero comparirà la scritta Game Over nella seconda riga. Inoltre nella seconda riga compariranno i vari simboli che si muoveranno da sinistra verso destra o viceversa.

![alt text](https://i0.wp.com/www.moreware.org/wp/wp-content/uploads/2020/04/gioco.png?resize=768%2C432&ssl=1)

Codice https://github.com/SimoneMoreWare/IR_game/blob/main/irgame.ino

Inoltre per chi fosse interessato è possibile creare una struttura 3D apposita per ospitare tutti i componenti e rendere il gioco più coinvolgente. Per coloro i quali sono intenzionati a creare tale struttura ecco qui il link github per farlo: https://github.com/LuigiMorelli/MoreWare/blob/master/Nano_con_LCD1602.zip

![alt text](https://i0.wp.com/www.moreware.org/wp/wp-content/uploads/2020/05/3d.jpg?resize=768%2C423&ssl=1)

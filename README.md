# Sasok

Tower Defense játék

A játékban két játékos harcol egymás ellen, mindkettejük célja a másik bázisának lerombolása.

Ennek eléréséhez egységeket indíthatnak, melyek a bázisuk mellől indulva automatikusan lépve az ellenfél bázisa felé haladnak,
azt elérve eggyel csökkentik a bázis életerejét, majd meghalnak. Adott mezőn több egység is tartózkodhat, mind ellenségesek,
mind ugyanahhoz a játékoshoz tartozók, ezt a mezőn látható szám mutatja.

Az egységek akadályozására mindkét játékosnak lehetősége van tornyok felállítására. A tornyok nem csak elállják az egység útját,
azaz nem haladhat át rajta, hanem megfelelő közelségből tüzet is nyitnak rá. Ez is automatikusan történik.
Az egységnek ha elfogy az életereje a tornyok tüzelése által, akkor meghal, mielőtt megsebezhette volna a bázist.
Egy mezőn csak egy torony állhat, ami onnantól fogva ott is marad a játék végéig, a másik játékos nem építhet már oda tornyot,
és nem is rombolhatja le azt.

Mindkét játékos rendelkezik kezdő aranytőkével, ami az idő múlásával fokozatosan gyarapszik, és levonódik belőle a megfelelő összeg,
ha a játékos egységet indít, tornyot épít. Aranyszerzésre még akkor kerül sor, ha a játékosnak meghal egy egysége.
Ha valakinek nincs elegendő aranya egy cselekvés végrehajtásához, várnia kell, míg feltöltődik az aranykészlete.

A játék menete úgy néz ki, hogy a két játékos a Beállításokban beállíthatják a pályaméretet, majd az Új Játék gomb megnyomásával indíthatják
a játékot. A pálya bal oldalán helyezkedik el a Piros játékos, neki bordó színű a bázisa, piros színű tornyokat rak le, és fehér
színű egységeket indít. A pálya jobb oldalán helyezkedik el a Zöld játékos, neki sötétzöld a bázisa, zöld színű tornyokat rak le,
és fekete színű egységeket indít.

A konkrét vezérlés: a piros játékos az egér bal gombjával tornyot helyezhet le adott mezőre, a 'p'-t nyomva egységet indíthat.
A zöld játékos a 'WASD' gombokkal lépkedve kiválaszthatja a mezőt, melyre tornyot akar építeni, ezen 'T' jelenik meg.
A ’t’ gomb megnyomására megépül a torony. Az ’e’ gomb megnyomására 1 egység indul a bázisából.

A játék kiírja, ha valakinek lerombolódott a bázisa, ekkor ő vesztett. Lehetőség van persze új játék kezdésére, és kilépésre is,
ez bármikor megtehető a játék folyamán.
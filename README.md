ZAgent
======

Move and resize OS X windows by pressing a key combination.


Usage
-----

Hold down `fn` and press one of the keys centred around `S` to move and resize the main window:

-   `fn` + `S` — Full screen
-   `fn` + `A` — Left half of screen
-   `fn` + `D` — Right half of screen
-   `fn` + `W` — Top half of screen
-   `fn` + `X` — Bottom half of screen
-   `fn` + `Q` — Top left quarter of screen
-   `fn` + `E` — Top right quarter of screen
-   `fn` + `Z` — Bottom left quarter of screen
-   `fn` + `C` — Bottom right quarter of screen

While holding down `fn`, press one of the number keys to specify the screen:

-   `fn` + `1` — Screen 1
-   `fn` + `2` — Screen 2
-   …
-   `fn` + `9` — Screen 9


### Advanced usage

The key combination continues as long as you hold down the `fn` key:

-   `fn` + `A`           — Left, ½ width
-   `fn` + `A`, `A`      — Left, ⅓ width
-   `fn` + `A`, `A`, `A` — Left, ¼ width
-   …

The first key pressed specifies one of the nine anchoring points, and sets the initial window size.  The final window size is a fraction of the initial size, with the number of keys pressed specifying the denominator:

-   `fn` + `W`           — Top, ½ height
-   `fn` + `W`, `W`      — Top, ⅓ height
-   `fn` + `W`, `W`, `W` — Top, ¼ height
-   …

Mix and match keys to achieve different results:

-   `fn` + `A`, `A`      — Left, ⅓ width, position 1
-   `fn` + `A`, `D`      — Left, ⅓ width, position 2
-   `fn` + `A`, `A`, `A` — Left, ¼ width, position 1
-   `fn` + `A`, `A`, `D` — Left, ¼ width, position 2
-   `fn` + `A`, `D`, `D` — Left, ¼ width, position 3
-   …

The `S` key is special, as it sets the initial window width to the width of the full screen:

-   `fn` + `S`           — Centre, ½ + ½ width
-   `fn` + `S`, `S`      — Centre, ½ + ⅓ width
-   `fn` + `S`, `S`, `S` — Centre, ½ + ¼ width
-   …


License
-------

BSD © [Miëtek Bak](http://mietek.io/)

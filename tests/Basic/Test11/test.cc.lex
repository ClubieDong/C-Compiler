     Token        Matched       Row  ColStart    ColEnd

       INT            int         1         1         4
   ID_TEXT        getchar         1         5        12
      CHAR              (         1        12        13
      CHAR              )         1        13        14
      CHAR              ;         1        14        15
       INT            int         2         1         4
   ID_TEXT        putchar         2         5        12
      CHAR              (         2        12        13
       INT            int         2        13        16
   ID_TEXT             ch         2        17        19
      CHAR              )         2        19        20
      CHAR              ;         2        20        21
      VOID           void         4         1         5
   ID_TEXT         output         4         6        12
      CHAR              (         4        12        13
       INT            int         4        13        16
   ID_TEXT              n         4        17        18
      CHAR              )         4        18        19
      CHAR              {         5         1         2
        IF             if         6         5         7
      CHAR              (         6         8         9
   ID_TEXT              n         6         9        10
      CHAR              <         6        11        12
  CONSTINT              0         6        13        14
      CHAR              )         6        14        15
      CHAR              {         7         5         6
   ID_TEXT        putchar         8         9        16
      CHAR              (         8        16        17
 CONSTCHAR            '-'         8        17        20
      CHAR              )         8        20        21
      CHAR              ;         8        21        22
   ID_TEXT              n         9         9        10
      CHAR              =         9        11        12
      CHAR              -         9        13        14
   ID_TEXT              n         9        14        15
      CHAR              ;         9        15        16
      CHAR              }        10         5         6
      ELSE           else        11         5         9
        IF             if        11        10        12
      CHAR              (        11        13        14
   ID_TEXT              n        11        14        15
        EQ             ==        11        16        18
  CONSTINT              0        11        19        20
      CHAR              )        11        20        21
      CHAR              {        12         5         6
   ID_TEXT        putchar        13         9        16
      CHAR              (        13        16        17
 CONSTCHAR            '0'        13        17        20
      CHAR              )        13        20        21
      CHAR              ;        13        21        22
    RETURN         return        14         9        15
      CHAR              ;        14        15        16
      CHAR              }        15         5         6
       INT            int        16         5         8
   ID_TEXT            num        16         9        12
      CHAR              [        16        12        13
  CONSTINT             20        16        13        15
      CHAR              ]        16        15        16
      CHAR              ,        16        16        17
   ID_TEXT            idx        16        18        21
      CHAR              =        16        22        23
  CONSTINT              0        16        24        25
      CHAR              ;        16        25        26
     WHILE          while        17         5        10
      CHAR              (        17        11        12
   ID_TEXT              n        17        12        13
        NE             !=        17        14        16
  CONSTINT              0        17        17        18
      CHAR              )        17        18        19
      CHAR              {        18         5         6
   ID_TEXT            num        19         9        12
      CHAR              [        19        12        13
   ID_TEXT            idx        19        13        16
      CHAR              ]        19        16        17
      CHAR              =        19        18        19
   ID_TEXT              n        19        20        21
      CHAR              %        19        22        23
  CONSTINT             10        19        24        26
      CHAR              ;        19        26        27
   ID_TEXT              n        20         9        10
DIV_ASSIGN             /=        20        11        13
  CONSTINT             10        20        14        16
      CHAR              ;        20        16        17
       INC             ++        21         9        11
   ID_TEXT            idx        21        11        14
      CHAR              ;        21        14        15
      CHAR              }        22         5         6
       FOR            for        23         5         8
      CHAR              (        23         9        10
       DEC             --        23        10        12
   ID_TEXT            idx        23        12        15
      CHAR              ;        23        15        16
   ID_TEXT            idx        23        17        20
        GE             >=        23        21        23
  CONSTINT              0        23        24        25
      CHAR              ;        23        25        26
       DEC             --        23        27        29
   ID_TEXT            idx        23        29        32
      CHAR              )        23        32        33
   ID_TEXT        putchar        24         9        16
      CHAR              (        24        16        17
   ID_TEXT            num        24        17        20
      CHAR              [        24        20        21
   ID_TEXT            idx        24        21        24
      CHAR              ]        24        24        25
      CHAR              +        24        26        27
 CONSTCHAR            '0'        24        28        31
      CHAR              )        24        31        32
      CHAR              ;        24        32        33
      CHAR              }        25         1         2
       INT            int        27         1         4
   ID_TEXT          input        27         5        10
      CHAR              (        27        10        11
      CHAR              )        27        11        12
      CHAR              {        28         1         2
       INT            int        29         5         8
   ID_TEXT              n        29         9        10
      CHAR              =        29        11        12
  CONSTINT              0        29        13        14
      CHAR              ,        29        14        15
   ID_TEXT              t        29        16        17
      CHAR              =        29        18        19
   ID_TEXT        getchar        29        20        27
      CHAR              (        29        27        28
      CHAR              )        29        28        29
      CHAR              ;        29        29        30
     WHILE          while        30         5        10
      CHAR              (        30        11        12
 CONSTCHAR            '0'        30        12        15
        LE             <=        30        16        18
   ID_TEXT              t        30        19        20
      CHAR              &        30        21        22
   ID_TEXT              t        30        23        24
        LE             <=        30        25        27
 CONSTCHAR            '9'        30        28        31
      CHAR              )        30        31        32
      CHAR              {        31         5         6
   ID_TEXT              n        32         9        10
      CHAR              =        32        11        12
   ID_TEXT              n        32        13        14
      CHAR              *        32        15        16
  CONSTINT             10        32        17        19
      CHAR              +        32        20        21
   ID_TEXT              t        32        22        23
      CHAR              -        32        24        25
 CONSTCHAR            '0'        32        26        29
      CHAR              ;        32        29        30
   ID_TEXT              t        33         9        10
      CHAR              =        33        11        12
   ID_TEXT        getchar        33        13        20
      CHAR              (        33        20        21
      CHAR              )        33        21        22
      CHAR              ;        33        22        23
      CHAR              }        34         5         6
    RETURN         return        35         5        11
   ID_TEXT              n        35        12        13
      CHAR              ;        35        13        14
      CHAR              }        36         1         2
      VOID           void        38         1         5
   ID_TEXT           main        38         6        10
      CHAR              (        38        10        11
      VOID           void        38        11        15
      CHAR              )        38        15        16
      CHAR              {        39         1         2
       INT            int        40         4         7
   ID_TEXT              i        40         8         9
      CHAR              ;        40         9        10
       INT            int        41         4         7
   ID_TEXT              j        41         8         9
      CHAR              ;        41         9        10
       INT            int        42         4         7
   ID_TEXT              k        42         8         9
      CHAR              ;        42         9        10
   ID_TEXT              i        44         4         5
      CHAR              =        44         5         6
  CONSTINT              5        44         6         7
      CHAR              ;        44         7         8
   ID_TEXT              j        45         4         5
      CHAR              =        45         5         6
   ID_TEXT              i        45         6         7
      CHAR              +        45         7         8
   ID_TEXT              j        45         8         9
      CHAR              ;        45         9        10
   ID_TEXT              k        46         4         5
      CHAR              =        46         5         6
   ID_TEXT              i        46         6         7
      CHAR              +        46         7         8
   ID_TEXT              j        46         8         9
      CHAR              +        46         9        10
   ID_TEXT              k        46        10        11
      CHAR              ;        46        11        12
   ID_TEXT         output        48         4        10
      CHAR              (        48        10        11
   ID_TEXT              i        48        11        12
      CHAR              )        48        12        13
      CHAR              ;        48        13        14
   ID_TEXT         output        49         4        10
      CHAR              (        49        10        11
   ID_TEXT              j        49        11        12
      CHAR              )        49        12        13
      CHAR              ;        49        13        14
   ID_TEXT         output        50         4        10
      CHAR              (        50        10        11
   ID_TEXT              k        50        11        12
      CHAR              )        50        12        13
      CHAR              ;        50        13        14
        IF             if        52         4         6
      CHAR              (        52         7         8
   ID_TEXT              i        52         8         9
        EQ             ==        52         9        11
   ID_TEXT              i        52        11        12
      CHAR              )        52        12        13
      CHAR              {        53         4         5
       INT            int        54         7        10
   ID_TEXT              i        54        11        12
      CHAR              ;        54        12        13
       INT            int        55         7        10
   ID_TEXT              j        55        11        12
      CHAR              ;        55        12        13
       INT            int        56         7        10
   ID_TEXT              k        56        11        12
      CHAR              ;        56        12        13
   ID_TEXT              i        57         7         8
      CHAR              =        57         8         9
  CONSTINT             10        57         9        11
      CHAR              ;        57        11        12
   ID_TEXT              j        58         7         8
      CHAR              =        58         8         9
   ID_TEXT              i        58         9        10
      CHAR              +        58        10        11
   ID_TEXT              j        58        11        12
      CHAR              ;        58        12        13
   ID_TEXT              k        59         7         8
      CHAR              =        59         8         9
   ID_TEXT              i        59         9        10
      CHAR              +        59        10        11
   ID_TEXT              j        59        11        12
      CHAR              +        59        12        13
   ID_TEXT              k        59        13        14
      CHAR              ;        59        14        15
   ID_TEXT         output        60         7        13
      CHAR              (        60        13        14
   ID_TEXT              i        60        14        15
      CHAR              )        60        15        16
      CHAR              ;        60        16        17
   ID_TEXT         output        61         7        13
      CHAR              (        61        13        14
   ID_TEXT              j        61        14        15
      CHAR              )        61        15        16
      CHAR              ;        61        16        17
   ID_TEXT         output        62         7        13
      CHAR              (        62        13        14
   ID_TEXT              k        62        14        15
      CHAR              )        62        15        16
      CHAR              ;        62        16        17
        IF             if        64         7         9
      CHAR              (        64        10        11
   ID_TEXT              j        64        11        12
      CHAR              <        64        13        14
   ID_TEXT              k        64        15        16
      CHAR              )        64        16        17
      CHAR              {        65         7         8
       INT            int        66        10        13
   ID_TEXT              j        66        14        15
      CHAR              ;        66        15        16
       INT            int        67        10        13
   ID_TEXT              k        67        14        15
      CHAR              ;        67        15        16
   ID_TEXT              j        69        10        11
      CHAR              =        69        12        13
   ID_TEXT              i        69        14        15
      CHAR              *        69        15        16
  CONSTINT              5        69        16        17
      CHAR              ;        69        17        18
   ID_TEXT              k        70        10        11
      CHAR              =        70        12        13
   ID_TEXT              i        70        14        15
      CHAR              *        70        15        16
  CONSTINT             10        70        16        18
      CHAR              ;        70        18        19
   ID_TEXT         output        73        10        16
      CHAR              (        73        16        17
   ID_TEXT              i        73        17        18
      CHAR              )        73        18        19
      CHAR              ;        73        19        20
   ID_TEXT         output        74        10        16
      CHAR              (        74        16        17
   ID_TEXT              j        74        17        18
      CHAR              )        74        18        19
      CHAR              ;        74        19        20
   ID_TEXT         output        75        10        16
      CHAR              (        75        16        17
   ID_TEXT              k        75        17        18
      CHAR              )        75        18        19
      CHAR              ;        75        19        20
      CHAR              }        76         7         8
   ID_TEXT         output        78         7        13
      CHAR              (        78        13        14
   ID_TEXT              i        78        14        15
      CHAR              )        78        15        16
      CHAR              ;        78        16        17
   ID_TEXT         output        79         7        13
      CHAR              (        79        13        14
   ID_TEXT              j        79        14        15
      CHAR              )        79        15        16
      CHAR              ;        79        16        17
   ID_TEXT         output        80         7        13
      CHAR              (        80        13        14
   ID_TEXT              k        80        14        15
      CHAR              )        80        15        16
      CHAR              ;        80        16        17
      CHAR              }        81         4         5
   ID_TEXT         output        83         4        10
      CHAR              (        83        10        11
   ID_TEXT              i        83        11        12
      CHAR              )        83        12        13
      CHAR              ;        83        13        14
   ID_TEXT         output        84         4        10
      CHAR              (        84        10        11
   ID_TEXT              j        84        11        12
      CHAR              )        84        12        13
      CHAR              ;        84        13        14
   ID_TEXT         output        85         4        10
      CHAR              (        85        10        11
   ID_TEXT              k        85        11        12
      CHAR              )        85        12        13
      CHAR              ;        85        13        14
      CHAR              }        86         1         2
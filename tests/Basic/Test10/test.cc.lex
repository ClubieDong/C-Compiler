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
       INT            int        38         1         4
   ID_TEXT            div        38         5         8
      CHAR              (        38         8         9
       INT            int        38         9        12
   ID_TEXT              x        38        13        14
      CHAR              )        38        14        15
      CHAR              {        39         1         2
       INT            int        40         6         9
   ID_TEXT         factor        40        10        16
      CHAR              ;        40        16        17
   ID_TEXT         factor        42         6        12
      CHAR              =        42        13        14
  CONSTINT             10        42        15        17
      CHAR              ;        42        17        18
    RETURN         return        43         6        12
      CHAR              (        43        13        14
   ID_TEXT              x        43        14        15
      CHAR              /        43        15        16
   ID_TEXT         factor        43        16        22
      CHAR              )        43        22        23
      CHAR              ;        43        23        24
      CHAR              }        44         1         2
       INT            int        46         1         4
   ID_TEXT            rem        46         5         8
      CHAR              (        46         8         9
       INT            int        46         9        12
   ID_TEXT              x        46        13        14
      CHAR              ,        46        14        15
       INT            int        46        16        19
   ID_TEXT              y        46        20        21
      CHAR              )        46        21        22
      CHAR              {        47         1         2
       INT            int        48         4         7
   ID_TEXT         factor        48         8        14
      CHAR              ;        48        14        15
   ID_TEXT         factor        50         4        10
      CHAR              =        50        11        12
  CONSTINT             10        50        12        14
      CHAR              ;        50        14        15
    RETURN         return        51         4        10
      CHAR              (        51        11        12
   ID_TEXT              y        51        12        13
      CHAR              -        51        14        15
      CHAR              (        51        16        17
   ID_TEXT         factor        51        17        23
      CHAR              *        51        23        24
   ID_TEXT              x        51        24        25
      CHAR              )        51        25        26
      CHAR              )        51        26        27
      CHAR              ;        51        27        28
      CHAR              }        52         1         2
      VOID           void        54         1         5
   ID_TEXT       digitize        54         6        14
      CHAR              (        54        14        15
       INT            int        54        15        18
   ID_TEXT              x        54        19        20
      CHAR              )        54        20        21
      CHAR              {        55         1         2
       INT            int        56         2         5
   ID_TEXT             dx        56         6         8
      CHAR              ;        56         8         9
       INT            int        57         8        11
   ID_TEXT             rx        57        13        15
      CHAR              ;        57        15        16
     WHILE          while        59         8        13
      CHAR              (        59        14        15
   ID_TEXT              x        59        15        16
      CHAR              >        59        17        18
  CONSTINT              0        59        19        20
      CHAR              )        59        20        21
      CHAR              {        60         7         8
   ID_TEXT             dx        61        10        12
      CHAR              =        61        13        14
   ID_TEXT            div        61        15        18
      CHAR              (        61        18        19
   ID_TEXT              x        61        19        20
      CHAR              )        61        20        21
      CHAR              ;        61        21        22
   ID_TEXT             rx        62         6         8
      CHAR              =        62         9        10
   ID_TEXT            rem        62        11        14
      CHAR              (        62        14        15
   ID_TEXT             dx        62        15        17
      CHAR              ,        62        17        18
   ID_TEXT              x        62        18        19
      CHAR              )        62        19        20
      CHAR              ;        62        20        21
   ID_TEXT         output        63        12        18
      CHAR              (        63        18        19
   ID_TEXT             rx        63        19        21
      CHAR              )        63        21        22
      CHAR              ;        63        22        23
   ID_TEXT              x        64         6         7
      CHAR              =        64         8         9
   ID_TEXT             dx        64        10        12
      CHAR              ;        64        12        13
      CHAR              }        65         8         9
      CHAR              }        66         1         2
       INT            int        69         1         4
   ID_TEXT      Fibonacci        69         5        14
      CHAR              (        69        14        15
       INT            int        69        15        18
   ID_TEXT             x1        69        19        21
      CHAR              )        69        21        22
      CHAR              {        70         1         2
       INT            int        71         2         5
   ID_TEXT            val        71         6         9
      CHAR              ;        71         9        10
        IF             if        73         2         4
      CHAR              (        73         5         6
   ID_TEXT             x1        73         6         8
        EQ             ==        73         9        11
  CONSTINT              1        73        12        13
      CHAR              )        73        13        14
   ID_TEXT            val        73        15        18
      CHAR              =        73        19        20
  CONSTINT              1        73        21        22
      CHAR              ;        73        22        23
        IF             if        74         3         5
      CHAR              (        74         6         7
   ID_TEXT             x1        74         7         9
        EQ             ==        74        10        12
  CONSTINT              2        74        13        14
      CHAR              )        74        14        15
   ID_TEXT            val        74        16        19
      CHAR              =        74        20        21
  CONSTINT              1        74        22        23
      CHAR              ;        74        23        24
        IF             if        75         2         4
      CHAR              (        75         5         6
   ID_TEXT             x1        75         6         8
      CHAR              >        75         9        10
  CONSTINT              2        75        11        12
      CHAR              )        75        12        13
      CHAR              {        75        14        15
   ID_TEXT            val        76         3         6
      CHAR              =        76         7         8
   ID_TEXT      Fibonacci        76         9        18
      CHAR              (        76        18        19
   ID_TEXT             x1        76        19        21
      CHAR              -        76        21        22
  CONSTINT              2        76        22        23
      CHAR              )        76        23        24
      CHAR              +        76        24        25
   ID_TEXT      Fibonacci        76        25        34
      CHAR              (        76        34        35
   ID_TEXT             x1        76        35        37
      CHAR              -        76        37        38
  CONSTINT              1        76        38        39
      CHAR              )        76        39        40
      CHAR              ;        76        40        41
      CHAR              }        77         2         3
   ID_TEXT       digitize        78         8        16
      CHAR              (        78        16        17
   ID_TEXT            val        78        17        20
      CHAR              )        78        20        21
      CHAR              ;        78        21        22
    RETURN         return        79         8        14
   ID_TEXT            val        79        15        18
      CHAR              ;        79        18        19
      CHAR              }        80         1         2
      VOID           void        82         1         5
   ID_TEXT           main        82         6        10
      CHAR              (        82        10        11
      VOID           void        82        11        15
      CHAR              )        82        15        16
      CHAR              {        83         1         2
       INT            int        84         8        11
   ID_TEXT              x        84        12        13
      CHAR              ;        84        13        14
       INT            int        85         8        11
   ID_TEXT         result        85        12        18
      CHAR              ;        85        18        19
   ID_TEXT              x        87         8         9
      CHAR              =        87        10        11
   ID_TEXT          input        87        12        17
      CHAR              (        87        17        18
      CHAR              )        87        18        19
      CHAR              ;        87        19        20
   ID_TEXT         result        88         8        14
      CHAR              =        88        15        16
   ID_TEXT      Fibonacci        88        17        26
      CHAR              (        88        26        27
   ID_TEXT              x        88        27        28
      CHAR              )        88        28        29
      CHAR              ;        88        29        30
   ID_TEXT         output        89         8        14
      CHAR              (        89        14        15
   ID_TEXT         result        89        15        21
      CHAR              )        89        21        22
      CHAR              ;        89        22        23
      CHAR              }        90         1         2

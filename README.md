## Çalıştırma
```bash
gcc main.c -o main.out
./main.out <FILOZOF_SAYISI>
```
*Derleme komutundan sonra warning mesajı verirse dikkate almayınız.*


## Açıklama
Program klasik **dining philosophers** problemini farklı bir açıdan ele almaktadır. Programda en başta kullanıcıdan filozof sayısını (n) argüman olarak almakta, daha sonra çeşitli değişkenler için hafıza ayırmaktadır (allocate).
Programda tek bir garson bulunmakta, bu garson 10ms'de bir tepsiyi kontrol ederek pilav bitmiş ise tepsiye (1,n) arasında rastgele sayıda porsiyon pilav eklemektedir. Eğer tepsi boş değil ise garson tepsinin boşalmasını beklemektedir. Tepsi boşken eline çubukları alan filozof var ise o da garsonun tepsiyi doldurmasını beklemektedir.

Garsonun her bir pilav eklemesi bir gün olarak simüle edilmiştir ve simülasyon 1000 gün sürmektedir. Her günün sonunda aç kalan filozoflar kontrol edilip 1000 günlük simülasyon sonunda garsonun eklediği toplam pilav miktarı, her bir filozofun yediği pilav miktarı ve en fazla kaç gün peş peşe aç kaldığı bilgileri ekrana getirilmektedir. Ayrıca programın devam ettiği bilgisini sunmak adına 100 günde bir simülasyon mesajı bastırılmaktadır.

1000 günlük simülasyonun ardından bilgiler ekrana verilmektedir. Ardından ise birinci filozofun threadinin priority değeri diğerlerininkinden yüksek verilmekte ve simülasyon aynen tekrar edilmektedir. İlk 1000 günlük simülasyondaki bilgiler aynen ekrana bastırılıp program sonlanmaktadır.

*Önemli detaylar kod içerisinde yorum satırı olarak belirtilmiştir.*


## Örnek Çıktı
```
./main.out 7          
Number of philosophers is: 7
First 1000 days with equal priorities...
Day 100 simulating...
Day 200 simulating...
Day 300 simulating...
Day 400 simulating...
Day 500 simulating...
Day 600 simulating...
Day 700 simulating...
Day 800 simulating...
Day 900 simulating...
Day 1000 simulating...
Total number of porsions given by waiter: 3883
Number of porsions eaten by philosopher 1: 552
Max hungry days of philosopher 1: 10
Number of porsions eaten by philosopher 2: 563
Max hungry days of philosopher 2: 8
Number of porsions eaten by philosopher 3: 568
Max hungry days of philosopher 3: 6
Number of porsions eaten by philosopher 4: 550
Max hungry days of philosopher 4: 8
Number of porsions eaten by philosopher 5: 576
Max hungry days of philosopher 5: 9
Number of porsions eaten by philosopher 6: 548
Max hungry days of philosopher 6: 8
Number of porsions eaten by philosopher 7: 573
Max hungry days of philosopher 7: 7


Restarting simulation with make first philosopher prior...
Day 100 simulating...
Day 200 simulating...
Day 300 simulating...
Day 400 simulating...
Day 500 simulating...
Day 600 simulating...
Day 700 simulating...
Day 800 simulating...
Day 900 simulating...
Day 1000 simulating...
Total number of porsions given by waiter: 3999
Number of porsions eaten by philosopher 1: 613
Max hungry days of philosopher 1: 8
Number of porsions eaten by philosopher 2: 579
Max hungry days of philosopher 2: 12
Number of porsions eaten by philosopher 3: 586
Max hungry days of philosopher 3: 7
Number of porsions eaten by philosopher 4: 581
Max hungry days of philosopher 4: 6
Number of porsions eaten by philosopher 5: 554
Max hungry days of philosopher 5: 10
Number of porsions eaten by philosopher 6: 579
Max hungry days of philosopher 6: 6
Number of porsions eaten by philosopher 7: 575
Max hungry days of philosopher 7: 7
```
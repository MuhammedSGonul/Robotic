# QTR-8RC ile Çizgi İzleyen Robot (PID Kontrol)

&nbsp;&nbsp;&nbsp;&nbsp; [PID Kontrol Sistemleri](https://en.wikipedia.org/wiki/PID_controller)'nin kullanıldığı alanlar, genellikle stabil değerler istenilen durumlardan doğar. Çizgi izleyen robot yaparken de bizim istediğimiz stabil durum, robotun çizgiyi tam olarak ortalayarak yoluna devam etmesi. Bunun için de [PID denkleminden](https://en.wikipedia.org/wiki/PID_controller#Mathematical_form) yararlanacağız:

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PIDEquation.png" height="39" width="442"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Ancak şuan için PID denkleminin bütün işlemleri ile işlem yapmak yerine P ve D (oran ve türev) işimizi tamamen görecektir. I (integral) değeri ile birlikte çalışırsak da yine güzel sonuç elde etmiş oluruz. 

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PDEquation.png" height="39" width="265"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Bu denklemde türev işleminde süreye göre hareket etmemiz de gerekiyor. Burada dikkat çekmek istediğim nokta diğer kontrol sistemlerine göre sistemimizdeki farklılık zamana göre doğruluk tepkisinden de ziyade konum tepkisi. Dolayısıyla zamanı dikkate almamamız robotun çizgiyi takip etme şeklinde pek bir değişiklik oluşturmayacak (X-Y grafiği düşünün: Y ekseni 0-5000 arası pozisyonu gösteriyor ve y = 2500 çizgisi setpoint, X ekseni ise sadece çizgi izleyen robot pisti. Orijin noktasından bir veri grafiği, robot pistte ilerlerken setpoint'e doğru yaklaşıyor).


    error = position - setPoint;  
    rateError = error - lastError;
    out = kp * error + kd * rateError;
    lastError = error;


&nbsp;&nbsp;&nbsp;&nbsp; [PID algoritmasını](https://en.wikipedia.org/wiki/PID_controller#Pseudocode) kendimize göre çıkış algoritmasına çevirdikten sonra Kp (oransal kazanç) ve Kd (türevsel kazanç) sabitlerini belirlememiz gerekiyor. Bu katsayıları belirlemede belirli bir formül yoktur, belli başlı metodlar vardır. Bu projede yalnızca PD kullanacağımız için ben PD kazançlarını manuel ayarlayacağız. Eğer I (integral) de kullanmış olsaydık o zaman [Ziegler-Nichols](https://en.wikipedia.org/wiki/Ziegler–Nichols_method), [Cohen-Coon](https://en.wikipedia.org/wiki/PID_controller#Cohen–Coon_parameters) vs. metodları uygulayarak kazançları belirleyebilirdik.


## Algoritmanın bu karışık kısmı bittiğine göre şimdi tamamen robotu yapmaya geçebiliriz.


Kullanılacak elektronik parçalar: 
* L298N Motor Sürücü Kartı
* QTR-8RC Kızılötesi Sensör
* 2x DC Motor
* Arduino
* 2S - 7.4V LiPo Batarya

Parçaları aşağıdaki diagrama göre bağlantılarını gerçekleştireceğiz.
<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/Diagram.png" height= "555" width= "768"></p>


Sırada yazılım kısmı var. İlk ihtiyacımız olan kızılötesi sensör için üreticisi olan [Pololu'nun yazdığı kütüphaneyi](https://github.com/pololu/qtr-sensors-arduino) indirip kurmak. 

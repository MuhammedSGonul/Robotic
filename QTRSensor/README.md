# QTR-8RC ile Çizgi İzleyen Robot (PID Kontrol)

&nbsp;&nbsp;&nbsp;&nbsp; [PID Kontrol Sistemleri](https://en.wikipedia.org/wiki/PID_controller)'nin kullanıldığı alanlar, genellikle stabil değerler istenilen durumlardan doğar. Çizgi izleyen robot yaparken de bizim istediğimiz stabil durum, robotun çizgiyi tam olarak ortalayarak yoluna devam etmesi. Bunun için de [PID denkleminden](https://en.wikipedia.org/wiki/PID_controller#Mathematical_form) yararlanacağız:

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PIDEquation.png" height="39" width="442"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Ancak şuan için PID denkleminin bütün işlemleri ile işlem yapmak yerine P ve D (oran ve türev) işimizi tamamen görecektir. I (integral) değeri ile birlikte çalışırsak da yine güzel sonuç elde etmiş oluruz. 

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PDEquation.png" height="39" width="265"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Bu denklemde türev işleminde süreye göre hareket etmemiz de gerekiyor. Burada dikkat çekmek istediğim nokta diğer kontrol sistemlerine göre sistemimizdeki farklılık zamana göre doğruluk tepkisinden de ziyade konum tepkisi. Dolayısıyla zamanı dikkate almamamız robotun çizgiyi takip etme şeklinde pek bir değişiklik oluşturmayacak (X-Y grafiği düşünün: Y ekseni 0-5000 arası pozisyonu gösteriyor ve y = 2500 çizgisi setpoint, X ekseni ise sadece çizgi izleyen robot pisti. Orijin noktasından bir veri grafiği, robot pistte ilerlerken setpoint'e doğru yaklaşıyor).

```python
    error = position - setPoint;  
    rateError = error - lastError;
    out = kp * error + kd * rateError;
    lastError = error;
```

&nbsp;&nbsp;&nbsp;&nbsp; [PID algoritmasını](https://en.wikipedia.org/wiki/PID_controller#Pseudocode) kendimize göre çıkış algoritmasına çevirdikten sonra Kp (oransal kazanç) ve Kd (türevsel kazanç) sabitlerini belirlememiz gerekiyor. Bu katsayıları belirlemede belirli bir formül yoktur, belli başlı metodlar vardır. Bu projede yalnızca PD kullanacağımız için ben PD kazançlarını manuel ayarlayacağız. Eğer I (integral) de kullanmış olsaydık o zaman [Ziegler-Nichols](https://en.wikipedia.org/wiki/Ziegler–Nichols_method), [Cohen-Coon](https://en.wikipedia.org/wiki/PID_controller#Cohen–Coon_parameters) vs. metodları uygulayarak kazançları belirleyebilirdik.

## Algoritmanın bu karışık kısmı bittiğine göre şimdi tamamen robotu yapmaya geçebiliriz.

### Elektronik: 
* L298N Motor Sürücü Kartı
* QTR-8RC Kızılötesi Sensör
* 2x DC Motor
* Arduino
* 2S - 7.4V LiPo Batarya

Parçaların aşağıdaki diagrama göre bağlantılarını gerçekleştireceğiz.
<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/Diagram.png" height= "555" width= "768"></p>

### Yazılım
&nbsp;&nbsp;&nbsp;&nbsp; İlk ihtiyacımız olan kızılötesi sensör için üreticisi olan [Pololu'nun yazdığı kütüphaneyi](https://github.com/pololu/qtr-sensors-arduino) indirip kurmak (Kod, version 4.0.0'a göre uyumlu). Ardından [LineFollower.ino](https://github.com/MuhammedSGonul/Arduino-rojects/blob/main/QTRSensor/LineFollower/LineFollower.ino) dosyasını açarak içerisinde bazı değişkenleri kendimize göre ayarlamamız gerekiyor. 
<br/><br/>

```c
    #define kp .1
    #define kd 4
```

&nbsp;&nbsp;&nbsp;&nbsp; P ve D kazançlarını, robotun pistte nasıl bir durum sergileyeceğini bilmediğimiz için ilk çalıştırmada <code>kp</code> ve <code>kd</code> değişkenlerinde herhangi bir değişiklik yapmamıza gerek yok.
<br/><br/>

```c
    int maxSpeed = 200, normalSpeed = 140, rightMotorSpeed, leftMotorSpeed;
```

&nbsp;&nbsp;&nbsp;&nbsp; Motorlarınızın modeline göre <code>maxSpeed</code> ve <code>normalSpeed</code> değişkenlerini ayarlayabilirsiniz.
<br/><br/>

```c
    #define NUM_SENSORS 6
    unsigned int sensorValues[NUM_SENSORS];
```

&nbsp;&nbsp;&nbsp;&nbsp; Eğer kullanacağınız sensör sayısı değişkenlik gösteriyorsa kullandığınız sensör sayısı kadar <code>NUM_SENSORS</code> değişkenine değer verin. Bu sayede <code>sensorValues</code> sensör sayısına göre bir dizi oluşturacak.
<br/><br/>

&nbsp;&nbsp;&nbsp;&nbsp; Değişkenlerde ayarlamaları yaptıktan sonra sırada <code>setup()</code> kısmında bazı ayarlamalar gerekiyor.
<br/><br/>

```c
    qtrrc.setTypeRC();
```

&nbsp;&nbsp;&nbsp;&nbsp; Kullandığımız sensör tipi dijital olduğundan <code>setTypeRC()</code> şeklinde çağırıyoruz. Analog sensör (QTR-8A) kullanılacaksa <code>setTypeAnalog()</code> fonksiyonunu çağırmamız gerekiyordu. Tabi bunun yanında da sensörün pinleri de analog girişlere takılmalı ve belirtilmeli.
<br/><br/>

```c
    qtrrc.setSensorPins((const uint8_t[]) {7, 8, 9, 10, 11, 12}, NUM_SENSORS);
```

&nbsp;&nbsp;&nbsp;&nbsp; Burada belirtilen pin numaraları baştan sona (7 ---> 12) pozisyonunu belirlemede yardımcı olacak şekilde 1. sensör 2. sensör... şeklinde baz alıyor. Sensör pin numaralarını doğru sırayla girdiğimizden emin olacağız. Pekâlâ pinleri tam tersi olarak (12 ---> 7) girersek de birazdan aşağıda belirteceğim durumu göz önünde bulundurarak yazılımımızda ufak çaplı bir değişiklik yapmış olacağız.
<br/><br/>

```c
    for (int i = 0; i < 70; i++){
      qtrrc.calibrate();
      digitalWrite(13,HIGH);
      delay(20);
      digitalWrite(13,LOW);
      delay(20);       
    }
```

&nbsp;&nbsp;&nbsp;&nbsp; En başta sensör pozisyonunu kalibre etme amaçlı olarak bir döngü içinde kalibrasyon yapacağız. Kalibre işlemi gerçekleşirken, bizim bunu görebilmemiz adına arduinonun üzerindeki ledi 20 ms aralıklarla yakıp söndürüyoruz. Bu da 20 + 20 = 40ms / döngü başına demek olur. 70 döngü * 40 ms = 2.8 saniye demek. Yani sensör 2.8 saniye boyunca kalibre edilecek.
<br/><br/>

&nbsp;&nbsp;&nbsp;&nbsp; Devamını gözden geçirip kontrol etmek adına <code>loop()</code> döngüsünü anlayalım.
<br/><br/>

```c
position = qtrrc.readLineBlack(sensorValues); 
```

&nbsp;&nbsp;&nbsp;&nbsp; QTR kütüphanesinden yararlanarak pozisyonunu okutmayı amaçlayacağız. Pistteki çizgiyi çektiğimiz bandın rengine göre iki farklı fonksiyon bulunmakta. Bunlardan <code>readLineBlack()</code> veya <code>readLineWhite()</code> fonksiyonlarını seçebilirsiniz. Fonksiyonu uygulayarak pozisyonunu atadığımızda, pozisyon 0 ile 5000 aralığında değer almış olacak. Pozisyon durumu da PID komutlarında işimize yarayacak.
<br/><br/>

```python
    error = position - setPoint;  
    rateError = error - lastError;
    out = kp * error + kd * rateError;
    lastError = error;
```

# Line following robot with QTR-8RC (PID CONTROL)

&nbsp;&nbsp;&nbsp;&nbsp; PID kontrol sistemlerinin kullanıldığı alanlar, genellikle stabil değerler istenilen durumlardan doğar. Çizgi izleyen robot yaparken de bizim istediğimiz stabil durum, robotun çizgiyi tam olarak ortalayarak yoluna devam etmesi. Bunun için de PID denkleminden yararlanacağız:

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PIDEquation.png" height="39" width="442"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Ancak şuan için PID denkleminin bütün işlemleri ile işlem yapmak yerine P ve D (oran ve türev) işimizi tamamen görecektir. I (integral) değeri ile birlikte çalışırsak da yine güzel sonuç elde etmiş oluruz. 

<p align="center"><img src="https://raw.githubusercontent.com/MuhammedSGonul/Arduino-Projects/main/QTRSensor/PDEquation.png" height="39" width="265"></p>

&nbsp;&nbsp;&nbsp;&nbsp; Bu denklemde türev işleminde süreye göre hareket etmemiz de gerekiyor. Burada dikkat çekmek istediğim nokta diğer kontrol sistemlerine göre sistemimizdeki farklılık zamana göre doğruluk tepkisinden de ziyade konum tepkisi. Dolayısıyla zamanı dikkate almamamız robotun çizgiyi takip etme şeklinde pek bir değişiklik oluşturmayacak. (X-Y grafiği düşünün: Y ekseni 0-5000 arası pozisyonu gösteriyor ve y = 2500 çizgisi setpoint, X ekseni ise sadece çizgi izleyen robot pisti. Orijin noktasından bir veri grafiği, robot pistte ilerlerken setpoint'e doğru yaklaşıyor).

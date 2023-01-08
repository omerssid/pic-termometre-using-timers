# pic-termometre-using-timers

Bu projede NTC ısı sensörü ve mikrodenetleyici kullanarak 80-110 santigrat derece arasında 1 derece hassasiyet ile sıcaklık ölçümü yapan bir sistem tasarlanmıştır.

NTC sensörünün değerleri normalde -50/+150 derece arası çalışacak şekilde tasarlanmıştır lakin bizim çalışma aralığımız +80/+110 derece arası; bu nedenle orta nokta olan 95 santigrat dereceyi baz alıp direncimizi look-up table kullanarak bu aralıkta en hassas ölçüm yapacak şekilde 680Ω olarak seçildi. Sıcaklık değerleri 3 haneli 7-SEG display ile santigrat derece biçiminde gösterilmektedir.

![Projenin proteus üzerinde görünümü](https://github.com/omerssid/pic-termometre-using-timers/blob/main/screenshots/devre.png?raw=true)

Nom: Sites Jails

Version: 1.0

Type: Application web

Plate-forme: PHP

Outils de design: HTML, CSS, JavaScript, Bootstrap(v5.3.3)

Fonctionnalité:
	- Bloquer un site (URL)
	- Ajouter un mot de passe à l'URL bloqué
	- Débloquer un URL

Systèmes: Linux

Dépendances: - iptables (installation: sudo apt install iptables)
			 - serveur web (apache, nginx, ...)

---------------------------------------------------------------------------------------------------

Remarque: Il faut donner une permission sur la commande iptables à l'utilisateur du serveur web
(www-data pour apache2) sans delande de mot de passe

Étapes:
1- Lancez la commande
sudo visudo

2- Ajouter la ligne (si vous utiliser apache2)
www-data ALL=(ALL:ALL) NOPASSWD: /usr/sbin/iptables

3- Enregistrer le fichier et lancer l'application dans le serveur web

---------------------------------------------------------------------------------------------------

Attention: Cette version comprends encore des bugs. N'essayer pas de bloquer l'URL où votre application
est lancée ou autres URLs dépendants
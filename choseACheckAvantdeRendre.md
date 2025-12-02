Messages d'erreur (Reply)
Comportement des fonctions (nc &&  irssi)
Leaks (valgrind)


Si un JOIN a lieu avec succès, on envoie à l'utilisateur le sujet du canal (en utilisant RPL_TOPIC) et la liste des utilisateurs du canal (en utilisant RPL_NAMREPLY), y compris lui-même.

Check le M

INVITE


Chose a modifier/ajouter sur JOIN :
	- -l (limite de personne dans le channel)
	- -k (juste a l'implimenter regarder note dans join.cpp)
	- -i (invite only), rien qui est fait (faire INVITE en prio, ajouter liste d'inviter dans channel)
	- Channel avec mdp (a verifier)
	- Reply JOIN

	^d cp de maykeul





	JOIN 
		- Check si le nom est valid
		- Check si le channel exist
			- si non creer
			- si oui 
				- check si mdp
					- si non join
					- si oui check mdp
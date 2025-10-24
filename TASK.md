REFERENCE : http://abcdrfc.free.fr/rfc-vf/rfc1459.html

Information importante :

1.2 : En plus du pseudonyme, tous les serveurs doivent connaître les informations suivantes sur tous les clients : le vrai nom de l'hôte sur lequel le client est exécuté, le nom de l'utilisateur du client sur cet hôte, et le serveur auquel le client est connecté. Le pseudo ne doit pas compter plus de 9 characters

1.2.1 : Operateur = Admin

1.3 : Le canal est creer quand quelqu'un le rejoinds et se detruit une fois qu'il n'y a plus personne (tout le monde peux le rejoindre du moment qu'il y a quelqu'un a l'interieur)

1.3 : Le Formatage	- Commence par '&' ou '#'
					- Max 200 characters
					- Pas de whiteSpace
					- Pas de "contrôle G (^G ou ASCII 7)"
					- Pas de ',' (est utilise pour separer les protocoles)

1.3 : Different cannaux :	- '#' connu de tous les serveurs
							- '&' connu du serveur du quelle il a ete creer

1.3 : La personne qui creer le canal devient operateur
	  Un utilisateur peux se connecter a plusieurs canaux mais conseiller de ne pas depasser 10

1.3.1 : Les commandes réservées aux opérateurs de canaux sont :
		KICK - Éjecte un client d'un canal
		MODE - Change le mode d'un canal
		INVITE - Invite un client dans un canal à accès sur invitation (mode +i)
		TOPIC - Change le titre du canal, dans un canal en mode +t

2.3 :	Les message ne doivent pas depaser 512 characters en comptant le CR-LF (retour chariot - saut de ligne) et doit toujours terminer par se dernier
		Une réponse numérique (numeric reply) est un message envoyé par le serveur (jamais par un client) qui utilise une commande composée d’un code à trois chiffres (ex. 001, 433).
		C’est la forme principale employée par les serveurs pour indiquer :

		Exemple:
			:source = préfixe du serveur (ex : :irc.example.com) — obligatoire pour les numerics.
			<NNN> = code à 3 chiffres (ex 001).
			<target> = habituellement le nick du client destinataire.
			[params] = paramètres spécifiques au code.
			:trailing = texte final libre, souvent le message lisible.

			:irc.example.com 001 Lucie :Welcome to the Internet Relay Network Lucie!\r\n

			Le préfixe est utilisé pour indiquer la véritable origine du message. S'il n'y a pas de préfixe, le message est considéré comme ayant pour origine la connexion de laquelle il est issu.

2.3.1 :	Le format de message en 'pseudo' BNF

		Le prefixe c'est soit nick!user@hote soit le nom du serv

		Les parties !utilisateur et @hôte sont optionnelles.
		👉 Les clients n’ont pas besoin d’envoyer de préfixe (le serveur sait déjà d’où ça vient).
		👉 Les serveurs, eux, doivent l’ajouter quand ils relaient un message.

3 :

 						  1--\
                              A        D---4
                          2--/ \      /
                                B----C
                               /      \
                              3        E
   Serveurs: A, B, C, D, E         Clients: 1, 2, 3, 4


3.1 :

Exemple 1 :
    Un message entre les clients 1 et 2 n'est vu que par le serveur A, qui l'envoie directement au client 2.
Exemple 2 :
    Un message entre les clients 1 et 3 est vu par les serveurs A & B, et par le client 3. Aucun autre client n'est autorisé à voir le message.
Exemple 3 :
    Un message entre les clients 2 et 4 n'est vu que par les serveurs A, B, C & D et par le client 4.

4.1 : Ordre de connection
	- PASS
	- NICK
	- USER

4.1.1 :

Le PASS doit etre defini avant chaque connection, seul l'admin peux le definir

4.1.2 : NICK
		Paramètres : <pseudonyme>
		Important de garder l'historique des psudo

4.1.3 : USER
		Commande: USER
		Paramètres: <nom d'utilisateur> <hôte> <nom de serveur> <nom réel> 

4.1.5 : OPER
		Commande: OPER
		Paramètres: <utilisateur> <mot de passe> 

4.1.6 : QUIT
		Commande: QUIT
		Paramètres: [<Message de départ >]      Part du serveur

4.2.1 : JOIN
		Commande: JOIN
		Paramètres: <canal>{,<canal>} [<clé>{,<clé>}] 

4.2.2 : PART
		Commande: PART
		Paramètres: <canal>						Part du channel

4.2.3.1 : MODE
		Paramètres: <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >] 

4.2.4 : TOPIC
		Commande: TOPIC
		Paramètres: <canal> [<sujet>] 

4.2.7 : INVITE
		Commande: INVITE
		Paramètres: <pseudonyme> <canal> 

4.2.8 : KICK
		Commande: KICK
		Paramètres: <canal> <utilisateur> [<commentaire>]

4.4.1 : PRIVMSG
		Commande: PRIVMSG
		Paramètres: <destinataire> <texte à envoyer >

4.6.1 : KILL
		Commande: KILL
		Paramètres: <pseudonyme> <commentaire> 
		(doublon pseudo)

8.5 :	Inclure le MOTD code msg 001 002 003 004

8.7 :	Terminaison des connexions serveur/client

8.9 :	Changement de pseudo


Step	: Se connecter au serveur
		: JOIN
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

		Le prefix c'est soit nick!user@hote soit le nom du serv

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
	- NICK/USER
	- USER/NICK

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









6.1 Réponses d'erreur

401 ERR_NOSUCHNICK
    "<pseudonyme> :No such nick/channel" 

Utilisé pour indiquer que le pseudonyme passé en paramètre à la commande n'est pas actuellement utilisé.

402 ERR_NOSUCHSERVER
    "<nom de serveur> :No such server" 

Utilisé pour indiquer que le nom du serveur donné n'existe pas actuellement.

403 ERR_NOSUCHCHANNEL
    "<nom de canal> :No such channel" 

Utilisé pour indiquer que le nom de canal donné est invalide.

404 ERR_CANNOTSENDTOCHAN
    "<nom de canal> :Cannot send to channel" 

Envoyé à un utilisateur qui (a) soit n'est pas dans un canal en mode +n ou (b) n'est pas opérateur (ou mode +v) sur un canal en mode +m ; et essaie d'envoyer un PRIVMSG à ce canal.

405 ERR_TOOMANYCHANNELS
    "<nom de canal> :You have joined too many channels" 

Envoyé à un utilisateur quand il a atteint le nombre maximal de canaux qu'il est autorisé à accéder simultanément, s'il essaie d'en rejoindre un autre.

406 ERR_WASNOSUCHNICK
    "<nom de canal> :There was no such nickname" 

Renvoyé par WHOWAS pour indiquer qu'il n'y a pas d'information dans l'historique concernant ce pseudonyme.

407 ERR_TOOMANYTARGETS
    "<destination> :Duplicate recipients. No message delivered" 

Renvoyé à un client qui essaie d'envoyer un PRIVMSG/NOTICE utilisant le format de destination utilisateur@hôte pour lequel utilisateur@hôte a plusieurs occurrences.

409 ERR_NOORIGIN
    ":No origin specified" 

Message PING ou PONG sans le paramètre origine qui est obligatoire puisque ces commandes doivent marcher sans préfixe.

411 ERR_NORECIPIENT
    ":No recipient given (<commande>)" 

Pas de destinataire.

412 ERR_NOTEXTTOSEND
    ":No text to send" 

Pas de texte à envoyer.

413 ERR_NOTOPLEVEL
    "<masque> :No toplevel domain specified" 

Domaine principal non spécifié.

414 ERR_WILDTOPLEVEL
    "<masque> :Wildcard in toplevel domain" 

Joker dans le domaine principal

Les erreurs 412-414 sont renvoyées par PRIVMSG pour indiquer que le message n'a pas été délivré. ERR_NOTOPLEVEL et ERR_WILDTOPLEVEL sont les erreurs renvoyées lors d'une utilisation invalide de "PRIVMSG $<serveur>" ou de "PRIVMSG #<hôte>".

421 ERR_UNKNOWNCOMMAND
    "<commande> :Unknown command" 

Renvoyé à un client enregistré pour indiquer que la commande envoyée est inconnue du serveur.

422 ERR_NOMOTD
    ":MOTD File is missing" 

Le fichier MOTD du serveur n'a pas pu être ouvert.

423 ERR_NOADMININFO
    "<serveur> :No administrative info available" 

Renvoyé par un serveur en réponse à un message ADMIN quand il y a une erreur lors de la recherche des informations appropriées.

424 ERR_FILEERROR
    ":File error doing <opération> on <fichier>"

Message d'erreur générique utilisé pour rapporter un échec d'opération de fichier durant le traitement d'un message.

431 ERR_NONICKNAMEGIVEN
    ":No nickname given" 

Renvoyé quand un paramètre pseudonyme attendu pour une commande n'est pas fourni.

432 ERR_ERRONEUSNICKNAME
    "<pseudo> :Erroneus nickname" 

Renvoyé après la réception d'un message NICK qui contient des caractères qui ne font pas partie du jeu autorisé. Voir les sections 1 et 2.2 pour les détails des pseudonymes valides.

433 ERR_NICKNAMEINUSE
    "<nick> :Nickname is already in use" 

Renvoyé quand le traitement d'un message NICK résulte en une tentative de changer de pseudonyme en un déjà existant.

436 ERR_NICKCOLLISION
    "<nick> :Nickname collision KILL" 

Renvoyé par un serveur à un client lorsqu'il détecte une collision de pseudonymes (enregistrement d'un pseudonyme qui existe déjà sur un autre serveur).

441 ERR_USERNOTINCHANNEL
    "<pseudo> <canal> :They aren't on that channel" 

Renvoyé par un serveur pour indiquer que l'utilisateur donné n'est pas dans le canal spécifié.

442 ERR_NOTONCHANNEL
    "<canal> :You're not on that channel" 

Renvoyé par le serveur quand un client essaie une commande affectant un canal dont il ne fait pas partie.

443 ERR_USERONCHANNEL
    "<utilisateur> <channel> :is already on channel" 

Renvoyé quand un client essaie d'inviter un utilisateur sur un canal où il est déjà.

444 ERR_NOLOGIN
    "<utilisateur> :User not logged in" 

Renvoyé par un SUMMON si la commande n'a pas pu être accomplie, car l'utilisateur n'est pas connecté.

445 ERR_SUMMONDISABLED
    ":SUMMON has been disabled" 

Renvoyé en réponse à une commande SUMMON si le SUMMON est désactivé. Tout serveur qui ne gère pas les SUMMON doit retourner cette valeur.

446 ERR_USERSDISABLED
    ":USERS has been disabled" 

Retourné en réponse à une commande USERS si la commande est désactivée. Tout serveur qui ne gère pas les USERS doit retourner cette valeur.

451 ERR_NOTREGISTERED
    ":You have not registered" 

Retourné par le serveur pour indiquer à un client qu'il doit être enregistré avant que ses commandes soient traitées.

461 ERR_NEEDMOREPARAMS
    "<commande> :Not enough parameters" 

Renvoyé par un serveur par de nombreuses commandes, afin d'indiquer que le client n'a pas fourni assez de paramètres.

462 ERR_ALREADYREGISTRED
    ":You may not reregister" 

Retourné par le serveur à tout lien qui tente de changer les détails enregistrés (tels que mot de passe et détails utilisateur du second message USER)

463 ERR_NOPERMFORHOST
    ":Your host isn't among the privileged" 

Renvoyé à un client qui essaie de s'enregistrer sur un serveur qui n'accepte pas les connexions depuis cet hôte.

464 ERR_PASSWDMISMATCH
    ":Password incorrect" 

Retourné pour indiquer l'échec d'une tentative d'enregistrement d'une connexion dû à un mot de passe incorrect ou manquant.

465 ERR_YOUREBANNEDCREEP
    ":You are banned from this server" 

Retourné après une tentative de connexion et d'enregistrement sur un serveur configuré explicitement pour vous refuser les connexions.

467 ERR_KEYSET
    "<canal> :Channel key already set" 

Clé de canal déjà définie.

471 ERR_CHANNELISFULL
    "<canal> :Cannot join channel (+l)" 

Impossible de joindre le canal (+l)

472 ERR_UNKNOWNMODE
    "<caractère> :is unknown mode char to me" 

Mode inconnu.

473 ERR_INVITEONLYCHAN
    "<canal> :Cannot join channel (+i)" 

Impossible de joindre le canal (+i).

474 ERR_BANNEDFROMCHAN
    "<canal> :Cannot join channel (+b)" 

Impossible de joindre le canal (+b).

475 ERR_BADCHANNELKEY
    "<canal> :Cannot join channel (+k)" 

Impossible de joindre le canal (+k).

481 ERR_NOPRIVILEGES
    ":Permission Denied- You're not an IRC operator" 

Toute commande qui requiert le privilège d'opérateur pour opérer doit retourner cette erreur pour indiquer son échec.

482 ERR_CHANOPRIVSNEEDED
    "<canal> :You're not channel operator" 

Toute commande qui requiert les privilèges 'chanop' (tels les messages MODE) doit retourner ce message à un client qui l'utilise sans être chanop sur le canal spécifié.

483 ERR_CANTKILLSERVER
    ":You cant kill a server!" 

Toute tentative d'utiliser la commande KILL sur un serveur doit être refusée et cette erreur renvoyée directement au client.

491 ERR_NOOPERHOST
    ":No O-lines for your host" 

Si un client envoie un message OPER et que le serveur n'a pas été configuré pour autoriser les connexions d'opérateurs de cet hôte, cette erreur doit être retournée.

501 ERR_UMODEUNKNOWNFLAG
    ":Unknown MODE flag" 

Renvoyé par un serveur pour indiquer que le message MODE a été envoyé avec un pseudonyme et que le mode spécifié n'a pas été identifié.

502 ERR_USERSDONTMATCH
    ":Cant change mode for other users" 
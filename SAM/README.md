# SAM (Satellite Abstraction Middleware)

Le SAM est l’interface (middleware) entre les satellites et le gestionnaire. 

Le gestionnaire ou système de gestion des circulations du réseau est constitué principalement d'une couche applicative qui gère un ensemble de zones interconnectées pour former une représentation sous forme de graphe du réseau physique. Certains nœuds de ce graphe sont des aiguillages dont la position définit les prédécesseurs et successeurs adjacents. Il connaît l'occupation des zones et le franchissement de barrières permettant d'affiner la position d'un train à l'intérieur d'une zone. Il gère le mouvement des aiguillages, mouvement qui ne peut être permis que si aucun train n'est ou ne sera en transit au travers de cet aiguillage. Il gère également l'état des feux qui protège les zones.

Le gestionnaire reçoit en entrée l'occupation des zones et les informations de franchissement des barrières. Il émet en sortie la position des aiguillages et l'état des feux.

Pour échanger ces informations et commandes, le gestionnaire a besoin du middleware SAM qui met en correspondance les éléments logiques (zones, barrières, aiguillages, feux) avec les élements physiques distants et la communication qui fait parvenir les commandes aux élements physiques distants et reçoit les états de ces mêmes élements physiques.

Cette communication utilise le réseau CAN.

Ce dossier SAM contient donc le middleware SAM sous forme d'une application Arduino minimum qu'il faudra compléter par votre application gestionnaire personnelle.

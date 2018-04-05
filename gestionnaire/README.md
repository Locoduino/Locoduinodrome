# Gestionnaire

Le gestionnaire est basé sur le système conçu par Pierre. Il est constitué principalement couche applicative qui gère un ensemble de zones interconnectées pour former une représentation sous forme de graphe du réseau physique. Certains nœuds de ce graphe sont des aiguillages dont la position définit les prédécesseurs et successeurs adjacents. Il connaît l'occupation des zones et le franchissement de barrières permettant d'affiner la position d'un train à l'intérieur d'une zone. Il gère le mouvement des aiguillages, mouvement qui ne peut être permis que si aucun train n'est ou ne sera en transit au travers de cet aiguillage. Il gère également l'état des feux qui protège les zones.

Le gestionnaire reçoit en entrée l'occupation des zones et les informations de franchissement des barrières. Il émet en sortie la position des aiguillages et l'état des feux.

Les deux autres couches sont un middleware qui met en correspondance les éléments logiques (zones, barrières, aiguillages, feux) avec les élements physiques distants et la communication qui fait parvenir les commandes aux élements physiques distants et reçoit les états de ces mêmes élements physiques.

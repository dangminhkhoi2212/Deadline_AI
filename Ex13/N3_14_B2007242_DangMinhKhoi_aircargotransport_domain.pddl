(define (domain air-cargo)

    (:requirements :strips)
    (:predicates
        (in ?obj ?place)(at ?obj ?place)(Cargo ?obj)(Plane ?obj)(Airport ?obj)
    )
    (:action LOAD
        :parameters (?c ?p ?a)
        :precondition (and (Cargo ?c)(Plane ?p)(Airport ?a)(at ?c ?a)(at ?p ?a))
        :effect (and (not(at ?c ?a))(in ?c ?p))
    )
    (:action UNLOAD
        :parameters (?c ?p ?a)
        :precondition (and (Cargo ?c)(Plane ?p)(Airport ?a)(in ?c ?p)(at ?p ?a))
        :effect (and (not(in ?c ?p))(at ?c ?a))
    )
    (:action FLY
        :parameters (?p ?from ?to)
        :precondition (and (Plane ?p)(Airport ?from)(Airport ?to)(at ?p ?from))
        :effect (and
            (at ?p ?to)
            (not (at ?p ?from))
        )
    )
)
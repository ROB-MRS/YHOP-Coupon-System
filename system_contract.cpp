#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("system_contract")]] system_contract : public eosio::contract
{

	public:

		using contract::contract;

		/* COSTRUTTORE DEL CONTRATTO */
		system_contract(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

		/* IMPLEMENTAZIONI DELLE ACTION */

		/* INSERIMENTO COUPON */
		[[eosio::action]]
		void addcoupon(int coupon_id, std::string pub_id, std::string beerlover_id, int value, std::string exp_date)
		{
			coupon_index coupons(get_self(), get_first_receiver().value);

			// verifica dell'esistenza del coupon nella tabella
			auto c_itr = coupons.find(coupon_id);
			check( c_itr == coupons.end() ,"Coupon gia' presente nel sistema!");

			// verifica della correttezza del campo value del coupon
			check( (value<100 && value>0) , "Il valore del Coupon e' inammissibile!" );

				// inserimento del record nella tebella
				coupons.emplace(_self, [&](auto& row) {
					row.coupon_id = coupon_id;
					row.pub_id = pub_id;
					row.beerlover_id = beerlover_id;
					row.value = value;
					row.exp_date = exp_date;
				});

		}


		/* INSERIMENTO REWARD */
		[[eosio::action]]
		void addreward(int reward_id, std::string hash)
		{
			reward_index rewards(get_self(), get_first_receiver().value);

			// verifica dell'esistenza del reward nella tabella
			auto r_itr = rewards.find(reward_id);
			check( r_itr == rewards.end() ,"Il Reward di questo Coupon e' gia' presente nel sistema!");

			// inserimento del record all'interno della tabella
			rewards.emplace(_self, [&](auto& row){
				row.reward_id = reward_id;
				row.hash = hash;
			});
		}

		/* CANCELLAZIONE COUPON */
		[[eosio::action]]
		void erasecoupon(int coupon_id)
		{
			coupon_index coupons(get_self(), get_first_receiver().value);
			
			// ricerca del record tramite l'id
			auto c_itr = coupons.find(coupon_id);

			// il record non esiste
			check( c_itr != coupons.end() ,"Il Coupon non esiste!");

			// eliminazione del record
			coupons.erase(c_itr);
		}

		/* CANCELLAZIONE REWARD */
		[[eosio::action]]
		void erasereward(int reward_id)
		{
			reward_index rewards(get_self(), get_first_receiver().value);
			
			// ricerca del record tramite l'id
			auto r_itr = rewards.find(reward_id);

			// il record non esiste
			check( r_itr != rewards.end() ,"Il Coupon non esiste!");

			// eliminazione del record
			rewards.erase(r_itr);
		}

	private:

		/* STRUTTURA DEL COUPON */
		struct [[eosio::table]] coupon {
			int coupon_id;
			std::string pub_id;
			std::string beerlover_id;
			int value;
			std::string exp_date;

			int primary_key() const{ return coupon_id;}
		};

		// tabella dei coupons
		using coupon_index = eosio::multi_index<"coupons"_n, coupon>;


		/* STRUTTURA DEL REWARD */
		struct [[eosio::table]] reward {
			int reward_id;
			std::string hash;

			int primary_key() const{ return reward_id;}
		};

		// tabella dei rewards
		using reward_index = eosio::multi_index<"rewards"_n, reward>;

};
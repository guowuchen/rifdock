// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington UW TechTransfer, email: license@u.washington.edu.



#ifndef INCLUDED_riflib_rif_RifGenerator_hh
#define INCLUDED_riflib_rif_RifGenerator_hh

#include <riflib/types.hh>
// #include <riflib/RotamerGenerator.hh>
#include <riflib/RifBase.hh>
#include <core/pose/Pose.hh>
#include <utility/vector1.hh>
#include <riflib/rifdock_typedefs.hh>
#include <string>

#ifdef USEGRIDSCORE
	#include <protocols/ligand_docking/GALigandDock/GridScorer.hh>
#endif

// namespace devel {
// namespace scheme {
// 	struct ScoreRotamerVsTarget;
// }
// }

// namespace scheme {
// namespace chemical {
// 	struct RotamerIndexSpec;
// }
// }

namespace devel {
namespace scheme {
namespace rif {

using ::devel::scheme::shared_ptr;

struct RifAccumulator { //声明一个结构体RifAccumulator
	virtual ~RifAccumulator(){} //RifAccumulator的析构函数
	virtual void insert( EigenXform const & x, float score, int rot, int sat1=-1, int sat2=-1, bool force=false, bool single_thread=false) = 0;
	virtual void report( std::ostream & out ) const = 0;
	virtual void checkpoint( std::ostream & out, bool force_override=false ) = 0;
	virtual uint64_t n_motifs_found() const = 0;
	virtual int64_t total_samples() const = 0;
	virtual void condense(bool force_override=false) = 0;
	virtual bool need_to_condense() const = 0;
	virtual shared_ptr<RifBase> rif() const = 0;
	virtual void clear() = 0; // seems to only clear temporary storage....
	virtual uint64_t count_these_irots( int irot_low, int irot_high ) const = 0;
	virtual std::set<size_t> get_sats_of_this_irot( devel::scheme::EigenXform const & x, int irot ) const = 0;
	virtual bool initialize_with_rif( shared_ptr<RifBase> & rif ) = 0;
};
typedef shared_ptr<RifAccumulator> RifAccumulatorP; //将RifAccumulator类型的指针取别名为RifAccumulator

struct RifGenParams { //声明一个结构体RifGenParams
  	core::pose::PoseOP             target = nullptr; //core::pose::PoseOP类型的指针
	std::string                    target_tag; //字符串target_tag
	std::string                    output_prefix="./default_"; //字符串输出后缀
	// set the fine-tuning file here, so it can control the rifgen processes of polar, apolar and hotspots.
  std::string																			 tuning_file="";
	utility::vector1<int>          target_res;
	//shared_ptr<RotamerIndex const> rot_index_p = nullptr;
	shared_ptr<RotamerIndex> rot_index_p = nullptr; //RotamerIndex类型的指针rot_index_p
	std::vector<std::string>       cache_data_path; //存放缓存数据路径的容器vector
	std::vector< VoxelArray* >     field_by_atype; //存放指针VoxelArray*的vector
	HBRayOpts                      hbopt;
#ifdef USEGRIDSCORE
	shared_ptr<protocols::ligand_docking::ga_ligand_dock::GridScorer> grid_scorer;
#endif
	// the only reason this is a pointer is to avoid including a bunch of stuff this high up
	shared_ptr<RifScoreRotamerVsTarget> rot_tgt_scorer; //RifScoreRotamerVsTarget类型的指针rot_tgt_scorer
};
typedef shared_ptr<RifGenParams> RifGenParamsP; //给RifGenParams类型的指针取别名为RifGenParamsP


struct RifGenerator {
	virtual ~RifGenerator(){}
	virtual void generate_rif(
		RifAccumulatorP accumulator,
		RifGenParamsP params
	) = 0;
	virtual void modify_rotamer_spec(::scheme::chemical::RotamerIndexSpec& rot_spec){}
};


}
}
}

#endif
